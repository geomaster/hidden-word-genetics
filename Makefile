hiddenword: main.cpp phenotype.hpp ecosystem.hpp
	clang++ -march=native -Wall -Werror -std=c++11 -o $@ -O3 -g $<
