import argparse


class ArgumentsInterpreter:
    arguments: dict

    def __init__(self):
        parser = argparse.ArgumentParser()
        parser.add_argument("-min_hp", type=int, help="Minimum HP for a vote to be considered.", default=0)
        parser.add_argument("-min_rp", type=int, help="Minimum reputation for a vote to be considered.", default=0)
        parser.add_argument("-budget", type=float, help="Hive amount to be distributed as prize.")
        self.arguments = vars(parser.parse_args())
