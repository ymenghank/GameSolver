// GameSolver.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <fstream>
#include <sstream>

#include "Game.h"


void print_result(std::ostream& os, Game& game) {
	os << "Value for Player I: \n";
	os << game.get_value() << '\n';
	os << '\n';

	os << "Optimal strategy for Player I: \n\n";
	std::list<Action> strat = game.get_strategy(true);
	for (std::list<Action>::iterator it = strat.begin(); it != strat.end(); ++it) {
		os << it->name << '\t' << it->probability << '\n';
	}
	os << '\n';

	os << "Optimal strategy for Player II: \n\n";
	strat = game.get_strategy(false);
	for (std::list<Action>::iterator it = strat.begin(); it != strat.end(); ++it) {
		os << it->name << '\t' << it->probability << '\n';
	}
	os << '\n';
}


int main(int argc, char** argv)
{
	std::list<std::string> X = std::list<std::string>();
	std::list<std::string> Y = std::list<std::string>();
	std::list<std::list<double>> A = std::list<std::list<double>>();

	std::string if_name = "";
	if (argc > 1) {
		if_name = std::string(argv[1]);
	} else if (if_name.size() == 0) {
		std::cout << "Enter input file name: ";
		std::cin >> if_name;
	}
	std::cout << "Loading file..." << std::endl;
	std::ifstream ifs(if_name);
	if (!ifs.is_open()) {
		std::cerr << "Cannot open file: " << if_name << std::endl;
		return 2;
	}
	bool first_row = true, first_column = true;
	std::string line;
	while (std::getline(ifs, line)) {
		std::stringstream ss(line);
		std::list<double> data_row = std::list<double>();
		first_column = true;
		while (!ss.eof()) {
			std::string cell;
			std::getline(ss, cell, '\t');
			if (first_row) {
				if (!first_column)
					Y.push_back(cell);
			} else if (first_column) {
				X.push_back(cell);
			} else {
				data_row.push_back(std::stod(cell));
			}
			first_column = false;
		}
		if (!first_row)
			A.push_back(data_row);
		first_row = false;
	}
	ifs.close();

	Game game = Game(X, Y, A);
	game.solve();

	std::cout << "Solved! Printing solution..." << std::endl << std::endl;
	if (argc > 2) {
		std::ofstream ofs(argv[2]);
		if (ofs.is_open()) {
			print_result(ofs, game);
			ofs.close();
		} else {
			print_result(std::cout, game);
		}
	} else {
		print_result(std::cout, game);
	}

	if (argc == 1) {
		// Shamelessly promotion
		std::cout << "Thanks for using GameSolver 1.0 by u/biowpn !" << std::endl;
		std::cout << "Press any key to exit";
		std::cin.ignore();
		std::cin.get();
	}

	return 0;
}

