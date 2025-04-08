#include "PSOOptimizer.h"
#include "AdamOptimizer.h"
#include "RegLoader.h"
#include <iostream>
#include <random>
#include <limits>
#include <cmath>

#ifdef max
#undef max
#endif


std::vector<double> PSOOptimizer::optimize(const OptimizationProblem& problem) {
    size_t dim = problem.x0.size();
    std::vector<std::vector<double>> positions(swarm_size, problem.x0);
    std::vector<std::vector<double>> velocities(swarm_size, std::vector<double>(dim, 0.0));
    std::vector<std::vector<double>> pbest = positions;
    std::vector<double> pbest_val(swarm_size, std::numeric_limits<double>::max());

    std::vector<double> gbest = problem.x0;
    double gbest_val = std::numeric_limits<double>::max();

    std::default_random_engine rng(std::random_device{}());
    std::uniform_real_distribution<double> urand(0.0, 1.0);
    std::uniform_real_distribution<double> range(-1.0, 1.0);

    for (auto& pos : positions)
        for (auto& x : pos) x += range(rng);  // 初始化扰动

    for (int iter = 0; iter < max_iter; ++iter) {
        for (int i = 0; i < swarm_size; ++i) {
            auto pred = problem.model->apply(positions[i]);
            double loss_val = problem.loss->evaluate(pred, problem.target);

            double reg_val = 0.0;
            if (problem.reg_handle && RegLoader::instance().eval_func) {
                std::vector<std::complex<double>> x_c(dim);
                for (size_t d = 0; d < dim; ++d) x_c[d] = std::complex<double>(positions[i][d], 0.0);
                reg_val = RegLoader::instance().eval_func(problem.reg_handle, x_c.data(), dim);
            }

            double total_val = loss_val + problem.reg_weight * reg_val;
            if (total_val < pbest_val[i]) {
                pbest[i] = positions[i];
                pbest_val[i] = total_val;
            }
            if (total_val < gbest_val) {
                gbest = positions[i];
                gbest_val = total_val;
            }
        }

        for (int i = 0; i < swarm_size; ++i) {
            for (size_t d = 0; d < dim; ++d) {
                double r1 = urand(rng), r2 = urand(rng);
                velocities[i][d] = inertia * velocities[i][d]
                    + c1 * r1 * (pbest[i][d] - positions[i][d])
                    + c2 * r2 * (gbest[d] - positions[i][d]);
                positions[i][d] += velocities[i][d];
            }
        }

        std::cout << "[PSO] Iter " << iter << ", best value = " << gbest_val << std::endl;
    }

    // 可选局部精修
    if (local_refine) {
        nlohmann::json cfg = { {"learning_rate", 0.01}, {"max_iter", 50}, {"tolerance", 1e-6} };
        AdamOptimizer local_opt(cfg);
        OptimizationProblem local_problem = problem;
        local_problem.x0 = gbest;
        gbest = local_opt.optimize(local_problem);
    }

    return gbest;
}
