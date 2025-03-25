class Player:
    def __init__(self, color, ai=False):
        self.ai = ai
        self.taken_stones = 0
        self.timer = 0
        self.color = color

    def handle_taken_stones(self):
        self.taken_stones += 2
        if self.taken_stones == 10:
            return True
        return False
