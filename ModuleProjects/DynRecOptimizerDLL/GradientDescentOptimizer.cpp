#include "GradientDescentOptimizer.h"
#include "RegLoader.h"
#include <iostream>

std::vector<double> GradientDescentOptimizer::optimize(const OptimizationProblem& problem) {
    std::vector<double> x = problem.x0;
    size_t N = x.size();

    int iter = 0;

    for (iter; iter < max_iter; ++iter) {
        // 1. A·x
        std::vector<std::complex<double>> pred = problem.model->apply(x);

        // 2. loss 和梯度
        double loss_val = problem.loss->evaluate(pred, problem.target);
        std::vector<std::complex<double>> loss_grad_complex = problem.loss->gradient(pred, problem.target);
        std::vector<double> grad_loss = problem.model->apply_adjoint(loss_grad_complex);

        // 3. 正则项和梯度
        double reg_val = 0.0;
        std::vector<double> grad_reg(N, 0.0);
        if (problem.reg_handle && RegLoader::instance().eval_func && RegLoader::instance().grad_func) {
            std::vector<std::complex<double>> x_complex(N);
            for (size_t i = 0; i < N; ++i) x_complex[i] = std::complex<double>(x[i], 0.0);

            reg_val = RegLoader::instance().eval_func(problem.reg_handle, x_complex.data(), N);
            std::vector<std::complex<double>> reg_grad_c(N);
            RegLoader::instance().grad_func(problem.reg_handle, x_complex.data(), reg_grad_c.data(), N);
            for (size_t i = 0; i < N; ++i) grad_reg[i] = reg_grad_c[i].real();
        }

        // 4. 合成目标值和梯度
        double total_val = loss_val + problem.reg_weight * reg_val;
        std::vector<double> grad(N);
        for (size_t i = 0; i < N; ++i)
            grad[i] = grad_loss[i] + problem.reg_weight * grad_reg[i];

        // 输出日志
        std::cout << "Iter " << iter << ": Loss = " << loss_val << ", Reg = " << reg_val << ", Total = " << total_val << std::endl;

        // 检查梯度范数是否收敛
        double norm_sq = 0.0;
        for (double g : grad) norm_sq += g * g;

        if (logger_) logger_->logIteration(iter, loss_val, std::sqrt(norm_sq));

        if (std::sqrt(norm_sq) < tolerance) {
            std::cout << "Converged with gradient norm < tolerance." << std::endl;
            break;
        }

        // 更新变量
        for (size_t i = 0; i < N; ++i)
            x[i] -= learning_rate * grad[i];
    }

    std::vector<std::complex<double>> x_c(N);
    for (size_t i = 0; i < N; ++i) x_c[i] = std::complex<double>(x[i], 0.0);
    double final_loss = problem.loss->evaluate(problem.model->apply(x), problem.target) +
        problem.reg_weight * RegLoader::instance().eval_func(problem.reg_handle, x_c.data(), N);
    if (logger_) logger_->logFinalResult(x, final_loss, iter, "completed");
    return x;
}