#include <iostream>
#include <stdlib.h>

#include "exampleConfig.h"
#include "example.h"

#include <cstdlib>

#include "tiny_dnn/tiny_dnn.h"

using namespace tiny_dnn;
// using namespace tiny_dnn::layers;
// using namespace tiny_dnn::activation;

/*
 * Simple main program that demontrates how access
 * CMake definitions (here the version number) from source code.
 */
int main() {
    std::cout << "C++ Boiler Plate v"
                << PROJECT_VERSION_MAJOR
                << "."
                << PROJECT_VERSION_MINOR
                << "."
                << PROJECT_VERSION_PATCH
                << "."
                << PROJECT_VERSION_TWEAK
                << std::endl;
    std::system("cat ../LICENSE");

    // input: 32x32x1 (1024 dimensions)  output: 10
    network<sequential> net;
    net << convolutional_layer(32, 32, 5, 1, 6) << tiny_dnn::tanh_layer() // 32x32in, conv5x5
        << average_pooling_layer(28, 28, 6, 2) << tiny_dnn::tanh_layer()  // 28x28in, pool2x2
        << fully_connected_layer(14 * 14 * 6, 120) << tiny_dnn::tanh_layer()
        << fully_connected_layer(120, 10);
    
    adagrad opt;
    
    int epochs = 50;
    int batch = 20;
//     net.fit<cross_entropy>(opt, x_data, y_data, batch, epochs);
//     
//     auto test_result = net.test(x_data, y_label);
//     auto loss = net.get_loss<cross_entropy>(x_data, y_data);
//     
//     auto y_vector = net.predict(x_data);
//     auto y_label = net.predict_max_label(x_data);
    
    net.save("my-network");
    
    // Bring in the dummy class from the example source,
    // just to show that it is accessible from main.cpp.
    Dummy d = Dummy();
    return d.doSomething() ? 0 : -1;
}
