#pragma once
#include <memory>
#include <complex>
#include <Eigen/Dense>

class ComplexLossFunction;
class RegularizerFunction;
class BaseOptimizer;

class OptimizerContext {
public:
    void setLossFunction(std::shared_ptr<ComplexLossFunction> loss);
    void setRegularizer(std::shared_ptr<RegularizerFunction> regularizer);
    void setOptimizer(std::shared_ptr<BaseOptimizer> optimizer);

    void setProblem(const Eigen::MatrixXcd& A, const Eigen::VectorXcd& b);
    void setInitialGuess(const Eigen::VectorXcd& x0);

    Eigen::VectorXcd run();

private:
    std::shared_ptr<ComplexLossFunction> loss_;
    std::shared_ptr<RegularizerFunction> regularizer_;
    std::shared_ptr<BaseOptimizer> optimizer_;

    Eigen::MatrixXcd A_;
    Eigen::VectorXcd b_;
    Eigen::VectorXcd x0_;
};