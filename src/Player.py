class Player:
    def __init__(self, color, ai=False):
        self.ai = ai
        self.taken_stones = 0
        self.timer = 0
        self.color = color
