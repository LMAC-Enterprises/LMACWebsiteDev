class ContestWinnerEntity:
    winningPlace: int
    artist: str
    imageUrl: str
    postUrl: str

    def __init__(self, winningPlace: str, artist: str, imageUrl: str, postUrl: str):
        self.winningPlace = winningPlace
        self.artist = artist
        self.imageUrl = imageUrl
        self.postUrl = postUrl

    def tojson(self):
        return {
            'winningPlace': self.winningPlace,
            'artist': self.artist,
            'imageUrl': self.imageUrl,
            'postUrl': self.postUrl
        }

    def __str__(self):
        return str(self.tojson())
