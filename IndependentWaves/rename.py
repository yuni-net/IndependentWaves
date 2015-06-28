#!/usr/bin/env python
# cording: UTF-8

import os
import shutil
import glob
import re

def main():
	current = r'C:\Users\MoNoQLoREATOR\Documents\projects\cpp\IndependentWaves\IndependentWaves' + '\\'
	files = os.listdir(current)
	for file in files:
		if file[-2:] != '.h':
			continue
		new_name = 'iw_' + file
		os.rename(file, new_name)
	return 0

main()