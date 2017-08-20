class GameState {
	constructor(levels, players) {
		this.levels = levels;
		this.players = players;
		if (levels === null) {
			this.levels = this.zeros([5,5]);
		}
		if (players === null) {
			this.players = [[[1,1], [3,3]], [[3,1], [1,3]]];
		}
	}

	zeros(dimensions) {
	    var array = [];
	    for (var i = 0; i < dimensions[0]; ++i) {
	        array.push(dimensions.length == 1 ? 0 : this.zeros(dimensions.slice(1)));
	    }

	    return array;
	}

	is_valid(oldX, oldY, newX, newY, player, piece) {
		if (newX >= 0 && newY >= 0 && newX < this.levels.length && newY < this.levels.length &&
			this.levels[newX][newY] < 4 && this.levels[newX][newY] <= this.levels[oldX][oldY] + 1 &&
			!this.occupied(newX, newY, player, piece)) {
			return true
		}
		return false
	}

	get_valid_blocks(x,y, player, piece) {
		var offsets = [-1, 0, 1]
		var blocks = []
		for(var i = 0; i < offsets.length; i++) {
			var offset1 = offsets[i]
			for (var j = 0; j < offsets.length; j++) {
				var offset2 = offsets[j]
				if (offset1 === offset2) {
					continue
				}
				var newX = x + i;
				var newY = y + j;
				if (newX >= 0 && newY >= 0 && newX < this.levels.length && newY < this.levels.length &&
					this.levels[newX][newY] < 4 && !this.occupied(newX, newY, player, piece)) {
					blocks.push([newX, newY])
				}
			}
		}
		return blocks
	}

	// Check if occupied not including given piece
	occupied(pos, player, piece) {
		for (var i = 0; i < this.players.length; i++) {
			var curr_player = this.players[i]
			for (var j = 0; j < curr_player.length; j++) {
				if (i === player && j === piece) {
					continue
				}
				else if (this.players[i][j] === pos) {
					return true
				}
			}
		}
		return false
	}

	moves(player) {
		var offsets = [-1, 0, 1];
		var moves = [];
		var curr_player = this.players[player];
		for (var piece = 0; piece < curr_player.length; piece++) {
			var x = curr_player[piece][0];
			var y = curr_player[piece][1];
			var curr_level = this.levels[x][y];
			for (var i = 0; i < offsets.length; i++) {
				var offset1 = offsets[i];
				for (var j = 0; j < offsets.length; j++) {
					var offset2 = offsets[j];
					if (offset1 === 0 && offset2 === 0) {
						continue;
					}
					var newX = x + offset1;
					var newY = y + offset2;
					if (this.is_valid(x,y, newX, newY, player, piece)) {
						var blocks = this.get_valid_blocks(newX, newY, player, piece)
						for (var k = 0; k < blocks.length; k++) {
							var block = blocks[k];
							moves.push([piece, newX, newY, block[0], block[1]]);
						}
					}
				}
			}
		}
		return moves;
	}

	make_move(newX, newY, blockX, blockY, player, piece) {
		this.players[player][piece] = [newX, newY];
		this.levels[blockX][blockY]++;
	}

	find_player(x,y) {
		/*
		Return player (number,piece) if found in (x,y), else null 
		*/
		for (var i = 0; i < this.players.length; i++) {
			var player = this.players[i];
			for (var j = 0; j < player.length; j++) {
				var player_pos = player[j];
				if (player_pos[0] === x && player_pos[1] === y) {
					return [i, j]
				}
			}
		}
		return null;
	}

	// print_board():
	// 	for i in xrange(len(self.levels)):
	// 		level = copy(self.levels[i])
	// 		for j in xrange(len(level)):
	// 			player = self.find_player((i,j))
	// 			if player is not None:
	// 				level[j] = str(player[0]) + '-' + str(player[1]) + '-' + str(level[j])
	// 			else:
	// 				level[j] = str(level[j]) + '    '
	// 		print level

	check_win() {
		/*
		Return winning player if game is over, else null
		*/
		for (var i = 0; i < this.players.length; i++) {
			var player = this.players[i];
			for (var j = 0; j < player.length; j++) {
				var player_pos = player[j]
				if (this.levels[player_pos[0]][player_pos[1]] === 3) {
					return i
				}
			}
		}
		return null
	}

	// def play(self):
	// 	curr_id = 0
	// 	player_ids = range(len(self.players))
	// 	while(True):
	// 		player = player_ids[curr_id]
	// 		# Check if only player left
	// 		if len(self.players) == 1:
	// 			print "Player %d won!" % player
	// 			break

	// 		moves = self.moves(player)
	// 		if len(moves) == 0:
	// 			player_ids.pop(player)
	// 			self.players.pop(player)
	// 			curr_id = (curr_id + 1) % (len(self.players))
	// 			continue

	// 		print "Player %d's turn" % player
	// 		self.print_board()
	// 		l = raw_input('piece,newPosX,newPosY,blockX,blockY: ').split(',')
	// 		l = [int(elem) for elem in l]
	// 		if len(l) != 5:
	// 			print "Bad input"
	// 			continue
	// 		piece, newX, newY, blockX, blockY = l[0], l[1], l[2], l[3], l[4]

	// 		if (piece, (newX, newY), (blockX, blockY)) in moves:
	// 			self.make_move((newX, newY), (blockX, blockY), player, piece)
	// 			win_player = self.check_win()
	// 			if win_player is not None:
	// 				print "Player %d won!" % win_player
	// 				break
	// 			curr_id = (curr_id + 1) % (len(self.players)) 
	// 		else:
	// 			print "Move is invalid, please try again"
}