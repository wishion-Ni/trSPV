#include "DEOptimizer.h"
#include "AdamOptimizer.h"
#include "RegLoader.h"
#include <iostream>
#include <random>
#include <limits>
#include <cmath>

#ifdef max
#undef max
#endif


std::vector<double> DEOptimizer::optimize(const OptimizationProblem& problem) {
    size_t dim = problem.x0.size();
    std::vector<std::vector<double>> population(pop_size, problem.x0);
    std::vector<double> fitness(pop_size, std::numeric_limits<double>::max());

    std::default_random_engine rng(std::random_device{}());
    std::uniform_real_distribution<double> urand(0.0, 1.0);
    std::uniform_int_distribution<int> index_rand(0, pop_size - 1);
    std::uniform_real_distribution<double> init_rand(-1.0, 1.0);

    for (auto& individual : population)
        for (auto& xi : individual) xi += init_rand(rng);

    for (int i = 0; i < pop_size; ++i) {
        auto pred = problem.model->apply(population[i]);
        double loss_val = problem.loss->evaluate(pred, problem.target);

        double reg_val = 0.0;
        if (problem.reg_handle && RegLoader::instance().eval_func) {
            std::vector<std::complex<double>> x_c(dim);
            for (size_t d = 0; d < dim; ++d) x_c[d] = std::complex<double>(population[i][d], 0.0);
            reg_val = RegLoader::instance().eval_func(problem.reg_handle, x_c.data(), dim);
        }
        fitness[i] = loss_val + problem.reg_weight * reg_val;
    }

    for (int iter = 0; iter < max_iter; ++iter) {
        for (int i = 0; i < pop_size; ++i) {
            // 选择 r1, r2, r3 ≠ i
            int r1, r2, r3;
            do { r1 = index_rand(rng); } while (r1 == i);
            do { r2 = index_rand(rng); } while (r2 == i || r2 == r1);
            do { r3 = index_rand(rng); } while (r3 == i || r3 == r1 || r3 == r2);

            std::vector<double> trial(dim);
            int j_rand = std::uniform_int_distribution<int>(0, dim - 1)(rng);
            for (size_t j = 0; j < dim; ++j) {
                if (urand(rng) < CR || j == j_rand)
                    trial[j] = population[r1][j] + F * (population[r2][j] - population[r3][j]);
                else
                    trial[j] = population[i][j];
            }

            // 评估 trial 个体
            auto pred = problem.model->apply(trial);
            double loss_val = problem.loss->evaluate(pred, problem.target);

            double reg_val = 0.0;
            if (problem.reg_handle && RegLoader::instance().eval_func) {
                std::vector<std::complex<double>> x_c(dim);
                for (size_t d = 0; d < dim; ++d) x_c[d] = std::complex<double>(trial[d], 0.0);
                reg_val = RegLoader::instance().eval_func(problem.reg_handle, x_c.data(), dim);
            }
            double total_val = loss_val + problem.reg_weight * reg_val;

            // 选择替代
            if (total_val < fitness[i]) {
                population[i] = trial;
                fitness[i] = total_val;
            }
        }

        double best_val = *std::min_element(fitness.begin(), fitness.end());
        if (logger_) logger_->logIteration(iter, best_val, 0.0);
    }

    // 获取最优解
    int best_idx = std::min_element(fitness.begin(), fitness.end()) - fitness.begin();
    std::vector<double> best = population[best_idx];

    // 可选局部精修
    if (local_refine) {
        nlohmann::json cfg = { {"learning_rate", 0.01}, {"max_iter", 50}, {"tolerance", 1e-6} };
        AdamOptimizer local_opt(cfg);
        OptimizationProblem local_problem = problem;
        local_problem.x0 = best;
        best = local_opt.optimize(local_problem);
    }

    if (logger_) logger_->logFinalResult(best, *std::min_element(fitness.begin(), fitness.end()), max_iter, "completed");
    return best;
}
