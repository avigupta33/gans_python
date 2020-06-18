from optimizer import AdamOptimizer
import math

class GAN:
    def __init__(self, optimizer_type):
        self.gen = Network
        self.dis = Network
        self.noise = noise
        self.training_data = training_data
        self.gen_optimizer = Optimizer(network, optimizer_type)
        self.dis_optimizer = Optimizer(network, optimizer_type)


    #https://arxiv.org/pdf/1406.2661.pdf
    def train(self, iterations: int, k: = 1, m: int = 16):
        for n in range(iterations):
            for step in range(k):
                noise_sample = noise.sample(m)
                training_sample = training_data.sample(m)

                discrim_products = dis.propagate(training_sample)

                dis_error = 0
                for i in range(m):
                    dis_error += math.log(discrim_products[i]) + \
                                math.log(1-dis.propagate(gen.propagate(noise_sample[i])))

                dis_grad = dis.get_gradient(dis_error)
                self.dis_optimizer.step(dis_grad)

            noise_sample = noise.sample(m)
            gen_error = 0
            for i in range(m):
                gen_error += math.log(1 - dis.propagate(gen.propagate(noise_sample[i])))

            gen_grad = gen.get_gradient(gen_error)
            self.gen_optimizer.step(gen_grad)










