#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

typedef std::vector<char> VS;
typedef std::vector<VS> VSS;
typedef std::unordered_map<std::string, std::pair<int, int>> UMAP_PAIR;
typedef std::vector<std::pair<int, int>> CORR;

struct game_input {
	int S = 0;
	int K = 0;
	int GW = 0;
	int GB = 0;
	int whitePawn = 0;
	int blackPawn = 0;
	char activePlayer = '0';
};

struct commands {
	std::string loadBoard = "LOAD_GAME_BOARD";
	std::string printBoard = "PRINT_GAME_BOARD";
	std::string doMove = "DO_MOVE";
};

void addToBorder(VSS& board, CORR& corrdinate){
	for (int i = 0; i < board.size(); ++i) {
		for (int j = 0; j < board[i].size(); j++) {
			if (i == 1 && (int)board[i][j] > 32) {
				corrdinate.push_back({ i, j });
			}
			else if (i == board.size() - 1 && (int)board[i][j] > 32) {
				corrdinate.push_back({ i, j });
			}
			else if ((int)board[i][j] > 32) {
				corrdinate.push_back({ i, j });
				break;
			}
		}
		for (int j = board[i].size() - 1; j >= 0; j--) {
			if ((int)board[i][j] > 32) {
				corrdinate.push_back({ i, j });
				break;
			}
		}
	}
}

void loadBoardGame(VSS& board, int K, int S, int& whiteOnBoard,int& blackOnBoard, int& validSign, bool& correctBoard,UMAP_PAIR& umap,CORR& corrdinate) {
	int n = 1;
	int size = 2 * S;
	std::string sLine;
	int SZ = S + 1;
	for (int j = 0; j < size; j++) {
		validSign = 0;
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
				whiteOnBoard++;
			}
			if (sign == 'B') {
				blackOnBoard++;
			}
			if (sign != ' ' && sign != '\t') {
				validSign++;
			}
			corrName = beginChar + std::to_string(beginIter);
			beginChar += 1;
			beginIter++;
			umap[corrName] = { j - 1, index };
		}
		if (j <= S && validSign == S + j - 1) {
			correctBoard = true;
		}
		else if (j > S && validSign == 2 * S - 1 - n) {
			correctBoard = true;
			n++;
		}
		else if (j != 0) {
			correctBoard = false;
			break;
		}
	}
	addToBorder(board, corrdinate);
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
void move(std::string& from, std::string& to, UMAP_PAIR& umap, VSS& board, CORR& corrdinate, char& active_player) {
	const auto& f = umap.find(to);
	int y = 0;
	int x = 0;
	const auto& ffrom = umap.find(from);
	int fy = 0;
	int fx = 0;
	if (ffrom != umap.end()) {
		fy = ffrom->second.first + 1;
		fx = ffrom->second.second;
		for (const auto& it : corrdinate) {
			if (it.first == fy && it.second == fx) {
				break;
			}
			else {
				continue;
			}
			//std::cout << "BAD_MOVE_<" << from << ">_IS_WRONG_STARTING_FIELD\n";
			return;
		}
		if (f != umap.end()) {
			y = f->second.first + 1;
			x = f->second.second + 1;
			if (board[y][x] == 'W' || board[y][x] == 'B') {
				std::cout << "BAD_MOVE_ROW_IS_FULL\n";
				return;
			}
			if (board[y][x] != '_' ||
				board[y][x] == active_player) {
				std::cout << "BAD_MOVE_<" << to << ">_IS_WRONG_DESTINATION_FIELD\n";
				return;
			}
		}
		else {
			std::cout << "BAD_MOVE_<" << to << "_IS_WRONG_INDEX\n";;
			return;
		}
	}
	else {
		std::cout << "BAD_MOVE_<" << from << "_IS_WRONG_INDEX\n";
		return;
	}
	board[y][x] = board[fy][fx];
	board[fy][fx] = active_player;
	std::cout << "MOVE_COMMITTED\n";
	active_player = getOpositTurn(active_player);
}


void printGameBoard(const VSS& board) {
	for (int j = 1; j < board.size(); j++) {
		for (int k = 0; k < board[j].size(); k++) {
			std::cout << board[j][k];
		}
		std::cout << "\n";
	}
	std::cout << "\n";
}

void findDirection(std::string& where, std::string& from, std::string& to){
	int idx = 0;
	while (idx < (int)where.size() && where[idx] != '-') {
		from += where[idx];
		idx++;
	}
	idx = idx + 1;
	while (idx < (int)where.size()) {
		to += where[idx];
		idx++;
	}
}

void showResultsOfLoading(int white_on_board, game_input& input, bool& correct_board, int black_on_board){
	if (white_on_board >(input.GW - input.whitePawn)) {
		std::cout << "WRONG_WHITE_PAWNS_NUMBER\n";
		correct_board = false;
	}
	else if (black_on_board > (input.GB - input.blackPawn)) {
		std::cout << "WRONG_BLACK_PAWNS_NUMBER\n";
		correct_board = false;
	}
	else if (correct_board == false) {
		std::cout << "WRONG_BOARD_ROW_LENGTH\n";
	}
	else
		std::cout << "BOARD_STATE_OK\n";
}

void showResultsOfPrinting(bool correct_board, game_input& input, VSS& board){
	if (correct_board == false)
		std::cout << "EMPTY_BOARD\n";
	else {
		std::cout << input.K << " " << input.S << " " << input.GW << " " << input.GB << "\n";
		std::cout << input.whitePawn << " " << input.blackPawn << " "
			<< input.activePlayer << "\n";
		printGameBoard(board);
	}
}

int main() {
	VSS board;
	bool correct_board = true;
	game_input input;
	commands commands;
	UMAP_PAIR position;
	CORR corrdinate;
	while (!std::cin.eof()) {
		int white_on_board = 0;
		int black_on_board = 0;
		int valid_char = 0;
		std::string command;
		std::cin >> command;
		if (command.compare(commands.loadBoard) == 0) {
			position.clear();
			corrdinate.clear();
			correct_board = true;
			std::cin >> input.S >> input.K >> input.GW >> input.GB >> input.whitePawn >>
				input.blackPawn >> input.activePlayer;
			int line = 0;
			int size = 2 * input.S;
			board = VSS(size);
			loadBoardGame(board, input.K, input.S, white_on_board, black_on_board,
				valid_char, correct_board, position, corrdinate);
			showResultsOfLoading(white_on_board, input, correct_board, black_on_board);
		}
		if (command.compare(commands.printBoard) == 0) {
			showResultsOfPrinting(correct_board, input, board);
		}
		if (command.compare(commands.doMove) == 0) {
			std::string where;
			std::cin >> where;
			std::string from = "";
			std::string to = "";
			findDirection(where, from, to);
			move(from, to, position, board, corrdinate, input.activePlayer);
		}
	}
}