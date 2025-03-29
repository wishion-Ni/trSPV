#include <iostream>
#include <memory>
#include <vector>
#include <iomanip>

// 引入误差函数头文件
#include "L2ErrorFunction.h"
#include "L1ErrorFunction.h"
#include "HuberErrorFunction.h"
#include "LogCoshErrorFunction.h"

void testErrorFunction(const ErrorFunction& errFunc,
    const std::vector<double>& prediction,
    const std::vector<double>& target) {
    std::cout << "Testing: " << errFunc.name() << std::endl;

    double err = errFunc.evaluate(prediction, target);
    std::vector<double> grad = errFunc.gradient(prediction, target);

    std::cout << "  Error Value = " << err << std::endl;
    std::cout << "  Gradient    = [ ";
    for (double g : grad) {
        std::cout << std::fixed << std::setprecision(3) << g << " ";
    }
    std::cout << "]\n\n";
}

int main() {
    // 构造测试数据
    std::vector<double> prediction = { 1.0, 2.0, 3.0, 10.0 };
    std::vector<double> target = { 1.5, 2.1, 2.9, 0.0 };  // 最后一个点是个离群点

    // 实例化误差函数
    L2ErrorFunction l2;
    L1ErrorFunction l1;
    HuberErrorFunction huber(1.0);  // delta 可调
    LogCoshErrorFunction logcosh;

    // 逐一测试
    testErrorFunction(l2, prediction, target);
    testErrorFunction(l1, prediction, target);
    testErrorFunction(huber, prediction, target);
    testErrorFunction(logcosh, prediction, target);

    return 0;
}
