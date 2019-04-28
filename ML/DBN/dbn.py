# -*- coding: utf-8 -*-
import numpy
from HiddenLayer import HiddenLayer
from utils import sigmoid
from rbm import RBM
from LogisticRegression import LogisticRegression


class DBN:

	def __init__(self, input=None, label=None, n_ins=2, hidden_layer_sizes=[3, 3], n_outs=2, rng=None):
		self.sigmoid_layers = []
		self.rbm_layers = []
		self.n_layers = len(hidden_layer_sizes)
		self.finetune_cost = None
		self.x = input
		self.y = label
		self.__construct_layers(n_ins, n_outs, hidden_layer_sizes, rng)

	def __construct_layers(self, n_ins, n_outs, hidden_layer_sizes, rng):
		for i in range(self.n_layers):
			# layer_input
			if i == 0:
				input_size = n_ins
				layer_input = self.x
			else:
				input_size = hidden_layer_sizes[i - 1]
				layer_input = self.sigmoid_layers[-1].sample_h_given_v()

			sigmoid_layer = HiddenLayer(input=layer_input, n_in=input_size, n_out=hidden_layer_sizes[i],
										rng=rng, activation=sigmoid)
			self.sigmoid_layers.append(sigmoid_layer)

			rbm_layer = RBM(input=layer_input, n_visible=input_size, n_hidden=hidden_layer_sizes[i], W=sigmoid_layer.W,
							hbias=sigmoid_layer.b)
			self.rbm_layers.append(rbm_layer)

			ay = self.sigmoid_layers[-1].sample_h_given_v()
			print("sigmoid_layers", ay.shape)
			self.lr_layer = LogisticRegression(input=self.sigmoid_layers[-1].sample_h_given_v(),
											   label=self.y, n_in=hidden_layer_sizes[-1], n_out=n_outs)
			self.finetune_cost = self.lr_layer.negative_log_likelihood()

	def pretrain(self, lr=0.1, k=1, epochs=100):
		# pre-train layer-wise
		layer_input = 0
		for i in range(self.n_layers):
			if i == 0:
				layer_input = self.x
			else:
				layer_input = self.sigmoid_layers[i - 1].sample_h_given_v(layer_input)
			rbm = self.rbm_layers[i]

			for epoch in range(epochs):
				rbm.contrastive_divergence(lr=lr, k=k, input=layer_input)
			# cost = rbm.get_reconstruction_cross_entropy()
			# print >> sys.stderr, \
			#        'Pre-training layer %d, epoch %d, cost ' %(i, epoch), cost

	def finetune(self, lr=0.1, epochs=100):
		layer_input = self.sigmoid_layers[-1].sample_h_given_v()

		# train log_layer
		epoch = 0
		done_looping = False
		while (epoch < epochs) and (not done_looping):
			self.lr_layer.train(lr=lr, input=layer_input)
			self.finetune_cost = self.lr_layer.negative_log_likelihood()
			# print >> sys.stderr, 'Training epoch %d, cost is ' % epoch, self.finetune_cost

			lr *= 0.95
			epoch += 1

	def predict(self, x):
		layer_input = x

		for i in range(self.n_layers):
			sigmoid_layer = self.sigmoid_layers[i]
			layer_input = sigmoid_layer.output(input=layer_input)

		out = self.lr_layer.predict(layer_input)
		return out


def run_test_dbn(pretrain_lr=0.1, pretraining_epochs=1000, k=1, finetune_lr=0.1, finetune_epochs=200):
	x = numpy.array([
		[1, 1, 1, 0, 0, 0],
		[1, 0, 1, 0, 0, 0],
		[1, 1, 1, 0, 0, 0],
		[0, 0, 1, 1, 1, 0],
		[0, 0, 1, 1, 0, 0],
		[0, 0, 1, 1, 1, 0]
	])
	y = numpy.array([
		[1, 0],
		[1, 0],
		[1, 0],
		[0, 1],
		[0, 1],
		[0, 1]
	])
	rng = numpy.random.RandomState(123)
	dbn = DBN(input=x, label=y, n_ins=6, hidden_layer_sizes=[3, 3], n_outs=2, rng=rng)
	# pre-training (TrainUnsupervisedDBN)
	dbn.pretrain(lr=pretrain_lr, k=1, epochs=pretraining_epochs)

	# fine-tuning (DBNSupervisedFineTuning)
	dbn.finetune(lr=finetune_lr, epochs=finetune_epochs)

	# test
	x = numpy.array([[1, 1, 0, 0, 0, 0],
					 [0, 0, 0, 1, 1, 0],
					 [1, 1, 1, 1, 1, 0]])

	print(dbn.predict(x))
	pass


if __name__ == "__main__":
	run_test_dbn()
