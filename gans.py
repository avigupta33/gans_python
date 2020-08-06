from Quantum import Matrix
from loss_functions import *
from layer import *
from network import *
import math
import random

if __name__ == "__main__":

    print("Hello, World!")
    input_vector = Matrix.uniform(dims=(10,), lower=-1, upper=1)
    layer1 = FCLayer(1, 1, 'TanH')
    layer2 = FCLayer(1, 1, 'LeakyReLU')
    network = Network([layer1, layer2])

    trainLen = 600
    validLen = 200
    testLen = 200
    inputs = []
    for i in range(trainLen + validLen + testLen):
        x = random.randint(0,100)
        inputs.append(
            (Matrix(x), Matrix(math.fabs(math.sin(x))))
        )

    train = inputs[:trainLen]
    valid = inputs[trainLen:trainLen + validLen]
    test = inputs[trainLen + validLen:]

    epochs = 10000
    alpha = 0.02


    total_loss_1 = 0
    count_1 = 0

    for data in test:
        output = network.forwards(data[0])
        loss = mse(data[1], output)
        count_1+=1
        total_loss_1 += loss.data[0]


    avg_test_loss_1 = total_loss_1/count_1


    for epoch in range(epochs):
        output = network.forwards(train[0][0])
        loss = mse(train[0][1], output)
        grads = network.backwards(loss)

        for i in range(1, len(train)):
            data = train[i]
            output = network.forwards(data[0])
            loss = mse(data[1], output)
            back = network.backwards(loss)
            print(back[0][0].data)
            print("Before", grads[0][0].data)
            new_grad = []
            new_grad.append((grads[0][0] + back[0][0], grads[0][1] + back[0][1]))
            new_grad.append((grads[1][0] + back[1][0], grads[1][1] + back[1][1]))
            grads = new_grad


            print("After", grads[0][0].data)
        network.layers[1].weights -= alpha * grads[0][0]

        network.layers[1].biases -= alpha * grads[0][1]
        network.layers[0].weights -= alpha * grads[1][0]
        network.layers[0].biases -= alpha * grads[1][1]

        if epoch % 500 == 0:
            val_loss = 0
            val_count = 0
            for data in valid:
                output = network.forwards(data[0])
                loss = mse(data[1], output)
                val_loss += loss.data[0]
                val_count+=1

            print(f"Epoch {epoch}: validation loss {val_loss/val_count}")

    total_loss_2 = 0
    count_2 = 0

    for data in test:
        output = network.forwards(data[0])
        loss = mse(data[1], output)
        count_2 += 1
        total_loss_2 += loss.data[0]

    avg_test_loss_2 = total_loss_2 / count_2

    print("1", avg_test_loss_1)
    print("2", avg_test_loss_2)









