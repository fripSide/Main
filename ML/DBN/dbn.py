# -*- coding: utf-8 -*-
import numpy
from HiddenLayer import HiddenLayer
from utils import sigmoid
from rbm import RBM
from LogisticRegression import LogisticRegression
import pandas as pd
from sklearn.preprocessing import MinMaxScaler
from sklearn.metrics.regression import r2_score, mean_squared_error

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

			# ay = self.sigmoid_layers[-1].sample_h_given_v()
			# print("sigmoid_layers", ay.shape, sigmoid_layer.W.shape)
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
				# print('Pre-training layer %d, epoch %d, cost ' %(i, epoch), cost)

	def finetune(self, lr=0.1, epochs=100):
		layer_input = self.sigmoid_layers[-1].sample_h_given_v()
		# train log_layer
		epoch = 0
		done_looping = False
		print("finetune", layer_input)
		while (epoch < epochs) and (not done_looping):
			self.lr_layer.train(lr=lr, input=layer_input)
			# self.finetune_cost = self.lr_layer.negative_log_likelihood()
			# print >> sys.stderr, 'Training epoch %d, cost is ' % epoch, self.finetune_cost

			lr *= 0.95
			epoch += 1

	def predict(self, x):
		layer_input = x

		for i in range(self.n_layers):
			sigmoid_layer = self.sigmoid_layers[i]
			layer_input = sigmoid_layer.output(input=layer_input)
		print("lr_layer layer_input", layer_input)
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


def normalize_data():
	"""将用电量归一化"""
	path = "E:\Projects\Main\ML\DBN\data.xlsx"
	df = pd.read_excel(path)
	df = df.drop(["日期"], axis=1)
	amount_len = 7  # 归一化用电量
	data = MinMaxScaler().fit_transform(df.iloc[:, :amount_len])
	df = pd.merge(pd.DataFrame(data), df.iloc[:, amount_len:], left_index=True, right_index=True)
	train_num = 79
	use_data = 16
	X_train = df.iloc[0:train_num, 1:]
	Y_train = df.iloc[0:train_num, :1]
	X_test = df.iloc[train_num:, 1:]
	Y_test = df.iloc[train_num:, :1]
	print("data size:", X_train.shape, Y_train.shape, X_test.shape, Y_test.shape)
	return X_train.values, Y_train.values, X_test.values, Y_test.values

def read_input_data():
	X_train, Y_train, X_test, Y_test = normalize_data()
	rng = numpy.random.RandomState(123)
	# print(normalized_X_train.values, normalized_Y_train)
	# print(normalized_X_train.shape, normalized_Y_train.shape)
	dbn = DBN(input=X_train, label=Y_train, n_ins=X_train.shape[1],
			  hidden_layer_sizes=[80] * 10, n_outs=1, rng=rng)
	dbn.pretrain(lr=0.001, k=1, epochs=1000)
	dbn.finetune(lr=0.001, epochs=200)
	resutls = dbn.predict(X_test)
	print("results", resutls, Y_test)
	print(Y_test.shape)
	print(r2_score(resutls, Y_test), mean_squared_error(resutls, Y_test))


if __name__ == "__main__":
	# run_test_dbn()
	read_input_data()
