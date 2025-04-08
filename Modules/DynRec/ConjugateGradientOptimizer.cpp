#include "ConjugateGradientOptimizer.h"
#include "RegLoader.h"
#include <iostream>
#include <cmath>

std::vector<double> ConjugateGradientOptimizer::optimize(const OptimizationProblem& problem) {
    std::vector<double> x = problem.x0;
    size_t N = x.size();

    std::vector<double> grad(N), grad_prev(N), direction(N);

    for (int iter = 0; iter < max_iter; ++iter) {
        // Forward model prediction
        auto pred = problem.model->apply(x);
        auto loss_grad_c = problem.loss->gradient(pred, problem.target);
        auto grad_loss = problem.model->apply_adjoint(loss_grad_c);

        // Regularization gradient
        std::vector<double> grad_reg(N, 0.0);
        if (problem.reg_handle && RegLoader::instance().grad_func) {
            std::vector<std::complex<double>> x_cplx(N);
            for (size_t i = 0; i < N; ++i) x_cplx[i] = std::complex<double>(x[i], 0.0);
            std::vector<std::complex<double>> grad_cplx(N);
            RegLoader::instance().grad_func(problem.reg_handle, x_cplx.data(), grad_cplx.data(), N);
            for (size_t i = 0; i < N; ++i) grad_reg[i] = grad_cplx[i].real();
        }

        for (size_t i = 0; i < N; ++i)
            grad[i] = grad_loss[i] + problem.reg_weight * grad_reg[i];

        // Compute beta based on selected CG strategy
        double beta = 0.0;
        if (iter > 0) {
            double num = 0.0, denom = 0.0;
            if (cg_variant == "FR") {
                for (size_t i = 0; i < N; ++i) {
                    num += grad[i] * grad[i];
                    denom += grad_prev[i] * grad_prev[i];
                }
                beta = (denom > 0) ? num / denom : 0.0;
            }
            else if (cg_variant == "PR") {
                for (size_t i = 0; i < N; ++i) {
                    num += grad[i] * (grad[i] - grad_prev[i]);
                    denom += grad_prev[i] * grad_prev[i];
                }
                beta = (denom > 0) ? num / denom : 0.0;
            }
            else if (cg_variant == "HS") {
                double numerator = 0.0, denominator = 0.0;
                for (size_t i = 0; i < N; ++i) {
                    double y_i = grad[i] - grad_prev[i];
                    numerator += grad[i] * y_i;
                    denominator += direction[i] * y_i;
                }
                beta = (denominator != 0.0) ? numerator / denominator : 0.0;
            }
        }

        // Compute direction
        for (size_t i = 0; i < N; ++i) {
            direction[i] = -grad[i] + beta * direction[i];
        }

        // Line search (simple fixed step)
        double alpha = 0.1;
        for (size_t i = 0; i < N; ++i)
            x[i] += alpha * direction[i];

        grad_prev = grad;

        // Convergence check
        double norm_sq = 0.0;
        for (double g : grad) norm_sq += g * g;
        if (std::sqrt(norm_sq) < tolerance) {
            std::cout << "[CG] Converged at iter " << iter << std::endl;
            break;
        }

        std::cout << "[CG] Iter " << iter << " grad norm = " << std::sqrt(norm_sq) << std::endl;
    }

    return x;
}
