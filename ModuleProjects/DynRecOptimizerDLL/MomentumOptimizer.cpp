#include "MomentumOptimizer.h"
#include "RegLoader.h"
#include <iostream>
#include <cmath>

std::vector<double> MomentumOptimizer::optimize(const OptimizationProblem& problem) {
    std::vector<double> x = problem.x0;
    size_t N = x.size();
    std::vector<double> velocity(N, 0.0);

    int iter = 0;

    for (iter; iter < max_iter; ++iter) {
        auto pred = problem.model->apply(x);
        auto loss_grad_c = problem.loss->gradient(pred, problem.target);
        auto grad_loss = problem.model->apply_adjoint(loss_grad_c);

        std::vector<double> grad_reg(N, 0.0);
        if (problem.reg_handle && RegLoader::instance().grad_func) {
            std::vector<std::complex<double>> x_c(N);
            for (size_t i = 0; i < N; ++i) x_c[i] = std::complex<double>(x[i], 0.0);
            std::vector<std::complex<double>> grad_c(N);
            RegLoader::instance().grad_func(problem.reg_handle, x_c.data(), grad_c.data(), N);
            for (size_t i = 0; i < N; ++i) grad_reg[i] = grad_c[i].real();
        }

        std::vector<double> grad(N);
        for (size_t i = 0; i < N; ++i)
            grad[i] = grad_loss[i] + problem.reg_weight * grad_reg[i];

        

        double norm = 0.0;
        for (double g : grad) norm += g * g;
        norm = std::sqrt(norm);

        std::vector<std::complex<double>> x_c(N);
        for (size_t i = 0; i < N; ++i)
            x_c[i] = std::complex<double>(x[i], 0.0);
        double loss_val = problem.loss->evaluate(pred, problem.target) +
            problem.reg_weight * RegLoader::instance().eval_func(problem.reg_handle, x_c.data(), N);
        if (logger_) logger_->logIteration(iter, loss_val, norm);

        if (norm < tolerance) break;
        
        for (size_t i = 0; i < N; ++i) {
            velocity[i] = momentum * velocity[i] - learning_rate * grad[i];
            x[i] += velocity[i];
        }
    }

    std::vector<std::complex<double>> x_c(N);
    for (size_t i = 0; i < N; ++i)
        x_c[i] = std::complex<double>(x[i], 0.0);
    double final_loss = problem.loss->evaluate(problem.model->apply(x), problem.target) +
        problem.reg_weight * RegLoader::instance().eval_func(problem.reg_handle, x_c.data(), N);
    if (logger_) logger_->logFinalResult(x, final_loss, iter, "completed");
    return x;
}
