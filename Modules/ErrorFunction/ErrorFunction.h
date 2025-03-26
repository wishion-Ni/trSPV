#pragma once

#include <vector>
#include <string>

/**
 * @brief 抽象误差函数基类，所有误差类型需继承此类。
 */
class ErrorFunction {
public:
    virtual ~ErrorFunction() {}

    /**
     * @brief 计算误差函数值 E(prediction, target)
     * @param prediction 模型预测值
     * @param target 实验/观测数据
     * @return 总误差值（标量）
     */
    virtual double evaluate(const std::vector<double>& prediction,
        const std::vector<double>& target) const = 0;

    /**
     * @brief 计算误差函数对预测值的一阶导数 ∂E/∂prediction
     * @param prediction 模型预测值
     * @param target 实验/观测数据
     * @return 与 prediction 同维度的梯度向量
     */
    virtual std::vector<double> gradient(const std::vector<double>& prediction,
        const std::vector<double>& target) const = 0;

    /**
     * @brief 获取误差函数名称标识
     * @return 字符串名称（如 "L2", "Huber" 等）
     */
    virtual const char* name() const = 0;
};
