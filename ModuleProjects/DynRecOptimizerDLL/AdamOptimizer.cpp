#include "AdamOptimizer.h"
#include "RegLoader.h"
#include <iostream>
#include <cmath>

std::vector<double> AdamOptimizer::optimize(const OptimizationProblem& problem) {
    std::vector<double> x = problem.x0;
    size_t N = x.size();
    std::vector<std::complex<double>> x_c(N);

    std::vector<double> m(N, 0.0);  // 一阶动量
    std::vector<double> v(N, 0.0);  // 二阶动量

    int iter = 1;

    for (iter; iter <= max_iter; ++iter) {
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

        // 更新一阶、二阶动量
        for (size_t i = 0; i < N; ++i) {
            m[i] = beta1 * m[i] + (1 - beta1) * grad[i];
            v[i] = beta2 * v[i] + (1 - beta2) * grad[i] * grad[i];
        }

        // 偏差修正 + 更新变量
        double beta1_t = std::pow(beta1, iter);
        double beta2_t = std::pow(beta2, iter);

        double norm = std::sqrt(std::inner_product(grad.begin(), grad.end(), grad.begin(), 0.0));
        for (size_t i = 0; i < N; ++i)
            x_c[i] = std::complex<double>(x[i], 0.0);

        double loss_val = problem.loss->evaluate(pred, problem.target) +
            problem.reg_weight * RegLoader::instance().eval_func(problem.reg_handle, x_c.data(), N);
        if (logger_) logger_->logIteration(iter, loss_val, norm);
        
        if (norm < tolerance) break;

        for (size_t i = 0; i < N; ++i) {
            double m_hat = m[i] / (1 - beta1_t);
            double v_hat = v[i] / (1 - beta2_t);
            double delta = learning_rate * m_hat / (std::sqrt(v_hat) + epsilon);
            x[i] -= delta;
            norm += grad[i] * grad[i];
        }
        
    }

    for (size_t i = 0; i < N; ++i)
    x_c[i] = std::complex<double>(x[i], 0.0);

    double final_loss = problem.loss->evaluate(problem.model->apply(x), problem.target) +
        problem.reg_weight * RegLoader::instance().eval_func(problem.reg_handle, x_c.data(), N);
    if (logger_) logger_->logFinalResult(x, final_loss, iter, "completed");

    return x;
}