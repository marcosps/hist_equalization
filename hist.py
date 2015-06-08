import os
import sys

max_pixel = 0
accumulated = []
width = 0
height = 0
cdf_min = 0
cdf_max = 0

# contains pixels and data
all_pixels = []
pixel_occurence = []

def errorAndExit(msg):
	print msg
	sys.exit(-1)

def cdf():
	global cdf_min, cdf_max, accumulated
	s = 0

	for i in xrange(0, max_pixel + 1):
		if pixel_occurence[i] > 0:
			s = s + pixel_occurence[i]

			if cdf_min == 0:
				cdf_min = s

			accumulated[i] = s

	cdf_max = s

def new_file():
	new_file = open('new.pgm', 'w')

	# write header of file
	new_file.write('P2\n')
	new_file.write('%d %d\n' % (width, height))
	new_file.write('%d\n' % max_pixel)

	for pix in all_pixels:
		if pix != '\n':
			tmp = accumulated[int(pix)] - cdf_min
			tmp = float(tmp) / float(width * height - cdf_min)
			tmp = int(round(tmp * max_pixel))
			if tmp < 0:
				print '< 0'

			pix = str(tmp) + ' '

		new_file.write(pix)

	new_file.close()

if len(sys.argv) < 2:
	errorAndExit('Needs a file as parameter')

filename = sys.argv[1]

if not os.path.exists(filename):
	errorAndExit('File not exists. Aborting')

f = open(filename, 'r')
lines = f.readlines()

# strip removes new line character
if lines[0].strip() != 'P2':
	print lines[0]
	errorAndExit('File type must be PGM P2 file format. Aborting')

# skip first line, as it's the file type
for line in lines[1:]:
	if line.startswith('#'):
		continue

	data = line.split(' ')

	# it must be the max pixel color information, and so, initilize the occurence lsit
	if len(data) == 1:
		tmp = data[0].strip()
		if not tmp.isdigit():
			print tmp
			errorAndExit('max digit is not actually a number. Aborting')
		max_pixel = int(tmp)
		pixel_occurence = [0] * (max_pixel + 1)
		accumulated = [0] * (max_pixel + 1)

	# must be dimensiob of the image
	elif len(data) == 2:
		w = data[0].strip()
		h = data[1].strip()
		if not w.isdigit() or not h.isdigit():
			errorAndExit('Width and height are not digits. Aborting')

		width, height = int(w), int(h)

	else:
		for d in data:
			if not d.isdigit() or d == '\n':
				continue

			# count the occurence pixels and collect pixel data into all_pixels list
			num = int(d)
			pixel_occurence[num] = pixel_occurence[num] + 1
			all_pixels.append(d)

		all_pixels.append('\n')

cdf()
new_file()
