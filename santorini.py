import fileinput
from copy import copy

levels = [[0 for i in xrange(5)] for j in xrange(5)]
players = [[(1,1), (3,3)], [(3,1), (1,3)]]

class GameState:
	def __init__(self, start, players):
		self.levels = levels
		self.players = players

	def is_valid(self, (oldX, oldY), (newX, newY), player, piece):
		if (newX >= 0 and newY >= 0 and newX < len(levels) and newY < len(levels) and
			self.levels[newX][newY] < 4 and self.levels[newX][newY] <= self.levels[oldX][oldY] + 1 and
			not self.occupied((newX, newY), player, piece)):
			return True
		return False

	def get_valid_blocks(self, (x,y), player, piece):
		offsets = [-1, 0, 1]
		blocks = []
		for i in offsets:
			for j in offsets:
				if i == 0 and j == 0:
					continue
				(newX, newY) = (x + i, y + j)
				if (newX >= 0 and newY >= 0 and newX < len(levels) and newY < len(levels) and
					self.levels[newX][newY] < 4 and not self.occupied((newX, newY), player, piece)):
					blocks.append((newX, newY))
		return blocks

	# Check if occupied not including given piece
	def occupied(self, pos, player, piece):
		for i in xrange(len(self.players)):
			curr_player = self.players[i]
			for j in xrange(len(curr_player)):
				if i == player and j == piece:
					continue
				elif self.players[i][j] == pos:
					return True
		return False

	def moves(self, player, ability=None):
		offsets = [-1, 0, 1]
		moves = []
		curr_player = self.players[player]
		for piece in xrange(len(curr_player)):
			(x, y) = self.players[player][piece]
			curr_level = self.levels[x][y]
			for i in offsets:
				for j in offsets:
					if i == 0 and j == 0:
						continue
					(newX, newY) = (x + i, y + j)
					if self.is_valid((x,y), (newX,newY), player, piece):
						blocks = self.get_valid_blocks((newX, newY), player, piece)
						for block in blocks:
							moves.append((piece, (newX, newY), block))
		return moves

	def make_move(self, newPos, blockPos, player, piece):
		self.players[player][piece] = newPos
		self.levels[blockPos[0]][blockPos[1]] += 1

	def find_player(self, pos):
		'''
		Return player (number,piece) if found in (x,y), else None 
		'''
		for i in xrange(len(self.players)):
			player = self.players[i]
			for j in xrange(len(player)):
				player_pos = player[j]
				if player_pos == pos:
					return (i, j)
		return None

	def print_board(self):
		for i in xrange(len(self.levels)):
			level = copy(self.levels[i])
			for j in xrange(len(level)):
				player = self.find_player((i,j))
				if player is not None:
					level[j] = str(player[0]) + '-' + str(player[1]) + '-' + str(level[j])
				else:
					level[j] = str(level[j]) + '    '
			print level

	def check_win(self):
		'''
		Return winning player if game is over, else None
		'''
		for i in xrange(len(self.players)):
			player = self.players[i]
			for j in xrange(len(player)):
				player_pos = player[j]
				if self.levels[player_pos[0]][player_pos[1]] == 3:
					return i
		return None

	def play(self):
		curr_id = 0
		player_ids = range(len(self.players))
		while(True):
			player = player_ids[curr_id]
			# Check if only player left
			if len(self.players) == 1:
				print "Player %d won!" % player
				break

			moves = self.moves(player)
			if len(moves) == 0:
				player_ids.pop(player)
				self.players.pop(player)
				curr_id = (curr_id + 1) % (len(self.players))
				continue

			print "Player %d's turn" % player
			self.print_board()
			l = raw_input('piece,newPosX,newPosY,blockX,blockY: ').split(',')
			l = [int(elem) for elem in l]
			if len(l) != 5:
				print "Bad input"
				continue
			piece, newX, newY, blockX, blockY = l[0], l[1], l[2], l[3], l[4]

			if (piece, (newX, newY), (blockX, blockY)) in moves:
				self.make_move((newX, newY), (blockX, blockY), player, piece)
				win_player = self.check_win()
				if win_player is not None:
					print "Player %d won!" % win_player
					break
				curr_id = (curr_id + 1) % (len(self.players)) 
			else:
				print "Move is invalid, please try again"

start = GameState(levels, players)
start.play()

