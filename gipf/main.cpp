#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

typedef std::vector<char> VS;
typedef std::vector<VS> VSS;
typedef std::unordered_map<std::string, std::pair<int, int>> UMAP_PAIR;
using namespace std;

struct game_input {
	int S = 0;
	int K = 0;
	int GW = 0;
	int GB = 0;
	int white_pawn = 0;
	int black_pawn = 0;
	char active_player = '0';
};

struct commands {
	string load_board = "LOAD_GAME_BOARD";
	string print_board = "PRINT_GAME_BOARD";
	string do_move = "DO_MOVE";
};

void load_game_board(VSS& board, int K, int S, int& white_on_board,
	int& black_on_board, int& valid_sign, bool& correct_board,
	UMAP_PAIR& umap,
	std::vector < std::pair<int, int>>& corrdinate) {
	int n = 1;
	int size = 2 * S;
	std::string sLine;
	int SZ = S + 1;
	for (int j = 0; j < size; j++) {
		valid_sign = 0;
		std::getline(std::cin, sLine);
		if (sLine == "")
			continue;
		for (int k = 0; k < (int)(sLine.size()); ++k) {
			board[j].push_back(sLine[k]);
		}
		std::istringstream iss(sLine);
		char sign;
		char beginChar = 'a';
		int beginIter = SZ;
		if (j < SZ) {
			beginChar = 'a';
			beginIter = SZ - j;
		}
		else {
			beginChar = ('a' + (j % SZ) + 1);
			beginIter = 1;
		}
		int index = -1;
		while (iss >> sign) {
			std::string corrName = "";
			index++;
			if (sign == ' ' || sign == '\t') {
				continue;
			}
			if (sign == 'W') {
				white_on_board++;
			}
			if (sign == 'B') {
				black_on_board++;
			}
			if (sign != ' ' && sign != '\t') {
				valid_sign++;
			}
			corrName = beginChar + std::to_string(beginIter);
			beginChar += 1;
			beginIter++;
			umap[corrName] = { j - 1, index };
		}
		if (j <= S && valid_sign == S + j - 1) {
			correct_board = true;
		}
		else if (j > S && valid_sign == 2 * S - 1 - n) {
			correct_board = true;
			n++;
		}
		else if (j != 0) {
			correct_board = false;
			break;
		}
	}
	// get border
	for (int i = 0; i < board.size(); ++i) {
		for (int j = 0; j < board[i].size(); j++) {
			if (i == 1 && (int)board[i][j] > 32) {
				corrdinate.push_back({ i, j });
				// std::cout << i << " " << j << " ";
			}
			else if (i == board.size() - 1 && (int)board[i][j] > 32) {
				corrdinate.push_back({ i, j });
				// std::cout << i << " " << j << " ";
			}
			else if ((int)board[i][j] > 32) {
				corrdinate.push_back({ i, j });
				// std::cout << i << " " << j << " ";
				break;
			}
		}
		for (int j = board[i].size() - 1; j >= 0; j--) {
			if ((int)board[i][j] > 32) {
				corrdinate.push_back({ i, j });
				// std::cout << i << " " << j << " ";
				break;
			}
		}
	}
}

char getOpositTurn(const char turn) {
	if (turn == 'W') {
		return 'B';
	}
	return 'W';
}
UMAP_PAIR getBorder(VSS& board) {
	UMAP_PAIR border;
	return border;
}
//<key, value> vale = pair -> first. second
void move(string& from, string& to, UMAP_PAIR& umap, VSS& board, std::vector<std::pair<int, int>>& corrdinate, char& active_player) {
	/* MOVE_COMMITTED - ruch poprawny, zatwierdzono zmiany na planszy i zmieniono aktualnego gracza.
		 BAD_MOVE_<x1>_IS_WRONG_INDEX - z³y indeks, pokazuje na pole spoza planszy.W przypadku kiedy podano wiêcej ni¿ jeden z³y indeks, drukowany jest tylko pierwszy napotkany.
		 UNKNOWN_MOVE_DIRECTION - nie mo¿na okreœliæ kierunku ruchu.
		 BAD_MOVE_<x1>_IS_WRONG_STARTING_FIELD - wybrano z³e, pole startowe(powinno to byæ jedno z po³o¿onych na obrze¿ach planszy)
		 BAD_MOVE_<x2>_IS_WRONG_DESTINATION_FIELD - wybrano z³e, pole docelowe(powinno to byæ pole wolne albo zawieraj¹ce pion któregoœ z graczy)
		 BAD_MOVE_ROW_IS_FULL - nie mo¿na wykonaæ ruchu bo wiersz jest zape³niony.*/
	const auto& f = umap.find(to);
	int y = 0;
	int x = 0;
	const auto& ffrom = umap.find(from);
	int fy = 0;
	int fx = 0;
	if (ffrom != umap.end()) {
		fy = ffrom->second.first + 1;
		fx = ffrom->second.second + 1;
		for (const auto& it : corrdinate) {
			if (it.first == fy && it.second == fx) {
				std::cout << "BAD_MOVE_<" << from << ">_IS_WRONG_STARTING_FIELD" << endl;
				return;
			}
		}
		if (f != umap.end()) {
			y = f->second.first + 1;
			x = f->second.second + 1;
			if (board[y][x] == 'W' || board[y][x] == 'B') {
				std::cout << "BAD_MOVE_ROW_IS_FULL" << endl;
				return;
			}
			if (board[y][x] != '_' ||
				board[y][x] == active_player) { // TODO: czy to oznacza ze pionek nie moze byc
				// tym samym pionkiem
				std::cout << "BAD_MOVE_<" << to << ">_IS_WRONG_DESTINATION_FIELD" << endl;;
				return;
			}
		}else {
			std::cout << "BAD_MOVE_<" << to << "_IS_WRONG_INDEX" << endl;;
			return;
		}
	}
	else {
		std::cout << "BAD_MOVE_<" << from << "_IS_WRONG_INDEX" << endl;;
		return;
	}
	board[y][x] = board[fy][fx - 1];
	board[fy][fx - 1] = active_player;
	std::cout << "MOVE_COMMITTED\n";
	active_player = getOpositTurn(active_player);
}

void print_game_board(const VSS& board) {
	for (int j = 1; j < board.size(); j++) {
		for (int k = 0; k < board[j].size(); k++) {
			cout << board[j][k];
		}
		cout << endl;
	}
	cout << endl;
}

int main() {
	VSS board;
	bool correct_board = true;
	game_input input;
	commands commands;
	char turn = 0;
	UMAP_PAIR position;
	std::vector<pair<int, int>> corrdinate;
	while (!cin.eof()) {
		int white_on_board = 0;
		int black_on_board = 0;
		int valid_char = 0;
		string command;
		cin >> command;
		if (command.compare(commands.load_board) == 0) {
			position.clear();
			corrdinate.clear();
			correct_board = true;
			cin >> input.S >> input.K >> input.GW >> input.GB >> input.white_pawn >>
				input.black_pawn >> input.active_player;
			int line = 0;
			int current_line = 0;
			int size = 2 * input.S;
			board = VSS(size);
			load_game_board(board, input.K, input.S, white_on_board, black_on_board,
				valid_char, correct_board, position, corrdinate);
			if (white_on_board > (input.GW - input.white_pawn)) {
				cout << "WRONG_WHITE_PAWNS_NUMBER" << endl;
				correct_board = false;
			}
			else if (black_on_board > (input.GB - input.black_pawn)) {
				cout << "WRONG_BLACK_PAWNS_NUMBER" << endl;
				correct_board = false;
			}
			else if (correct_board == false) {
				cout << "WRONG_BOARD_ROW_LENGTH" << endl;
			}
			else
				cout << "BOARD_STATE_OK" << endl;
		}
		if (command.compare(commands.print_board) == 0) {
			if (correct_board == false)
				cout << "EMPTY_BOARD" << endl;
			else {
				cout << input.K << " " << input.S << " " << input.GW << " " << input.GB
					<< endl;
				cout << input.white_pawn << " " << input.black_pawn << " "
					<< input.active_player << endl;
				print_game_board(board);
			}
		}
		if (command.compare(commands.do_move) == 0) {
			std::string where;
			cin >> where;
			std::string from = "";
			int idx = 0;
			while (idx < (int)where.size() && where[idx] != '-') {
				from += where[idx];
				idx++;
			}
			idx = idx + 1;
			std::string to = "";
			while (idx < (int)where.size()) {
				to += where[idx];
				idx++;
			}
			move(from, to, position, board, corrdinate, input.active_player);
			// std::cout << "[DEUBG] " << from << " " << to << std::endl;
		}
	}
}