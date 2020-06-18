from optimizer import AdamOptimizer
import math

class GAN:
    def __init__(self, gen_optimizer, optimizer_type):
        self.gen = Network
        self.dis = Network
        self.noise = noise
        self.training_data = training_data
        self.gen_optimizer = gen_optimizer
        self.dis_optimizer = dis_optimizer

    def train(self, iterations: int, k: = 1, m: int = 16):
        for n in range(iterations):
            for step in range(k):
                noise_sample = noise.sample(m)
                training_sample = training_data.sample(m)

                discrim_products = dis.propagate(training_sample)

                dis_grad = 0
                for i in range(m):
                    dis_grad += math.log(discrim_products[i]) + \
                                math.log(1-dis.propagate(gen.propagate(noise[i])))

                self.dis_optimizer.step(dis_grad)

            noise_sample = noise.sample(m)
            gen_grad = 0
            for i in range(m):
                gen_grad += math.log(1 - dis.propagate(gen.propagate(noise_sample[i])) )
            self.gen_optimizer.step(gen_grad)










