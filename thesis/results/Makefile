all: graphs.pdf

graphs.pdf: graphs.dvi
	dvipdf graphs.dvi

graphs.dvi: graphs.tex images_eps
	latex graphs.tex

images_eps:
	gnuplot < plot.gplot

clean:
	rm -f graphs.aux graphs.log graphs.dvi graphs.pdf
	rm -f *.eps
