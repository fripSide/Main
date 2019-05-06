# -*- coding: utf-8 -*-
import pandas as pd
from sklearn.preprocessing import MinMaxScaler
from sklearn.metrics.regression import r2_score, mean_squared_error, mean_absolute_error

import numpy as np

np.random.seed(1337)  # for reproducibility
from sklearn.model_selection import train_test_split
from sklearn.metrics.regression import r2_score, mean_squared_error
from dbn.tensorflow import SupervisedDBNRegression
import tensorflow as tf


def normalize_data():
	"""将用电量归一化"""
	path = r"E:\Projects\Main\ML\DBN\data.xlsx"
	df = pd.read_excel(path)
	df = df.drop(["日期"], axis=1)
	amount_len = 7  # 归一化用电量
	# data = MinMaxScaler().fit_transform(df.iloc[:, :amount_len])
	# df = pd.merge(pd.DataFrame(data), df.iloc[:, amount_len:], left_index=True, right_index=True)
	df = pd.DataFrame(MinMaxScaler().fit_transform(df))
	# print(df.iloc[:, :1])
	# exit(-1)
	all_len = df.shape[0]
	train_num = 1000
	use_data = 16
	X_train = df.iloc[0:train_num, 1:]
	Y_train = df.iloc[0:train_num, :1]
	Y_train = pd.merge(Y_train, pd.DataFrame(np.zeros((all_len, 1))), left_index=True, right_index=True)
	X_test = df.iloc[train_num:, 1:]
	Y_test = df.iloc[train_num:, :1]
	# print(Y_test)
	Y_test = Y_test.reset_index()
	Y_test = Y_test.drop(["index"], axis=1)
	print(Y_test)
	Y_test = pd.merge(Y_test, pd.DataFrame(np.zeros((13, 1))), left_index=True, right_index=True)
	# Y_test = Y_test.drop(["index"])
	print(X_test.values)
	print(Y_test.values)
	print("data size:", X_train.shape, Y_train.shape, X_test.shape, Y_test.shape)
	# exit(-1)
	return X_train.values, Y_train.values, X_test.values, Y_test.values


def run_dbn():
	X_train, Y_train, X_test, Y_test = normalize_data()
	regressor = SupervisedDBNRegression(hidden_layers_structure=[100],
										learning_rate_rbm=0.01,
										learning_rate=0.01,
										n_epochs_rbm=20,
										n_iter_backprop=200,
										batch_size=16,
										activation_function='relu')
	# Y_train = np.reshape(Y_train, (79, 2))
	print(X_train.shape, Y_train.shape)

	regressor.fit(X_train, Y_train)

	# Test
	Y_pred = regressor.predict(X_test)
	Y_pred = np.delete(Y_pred, np.s_[-1:], axis=1)
	Y_test = np.delete(Y_test, np.s_[-1:], axis=1)
	print("Y_pred", Y_pred)
	print("Y_test", Y_test)
	print('Done.\nR-squared: %f\nMSE: %f' % (r2_score(Y_test, Y_pred), mean_squared_error(Y_test, Y_pred)))
	# print("mean_absolute_error", mean_absolute_error(Y_test, Y_pred) / Y_test.shape[0])
	# print("len", Y_test.shape[0])
	abs_sum = 0
	for i in range(0, 13):
		v1 = Y_pred[i][0]
		v2 = Y_test[i][0]
		print(v1, v2)
		abs_sum += abs(v1 - v2)
	print("final: ", abs_sum, abs_sum / 13)

# https://github.com/albertbup/deep-belief-network/blob/master/examples/regression_demo.py
if __name__ == "__main__":
	run_dbn()
