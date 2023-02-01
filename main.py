from level import frame_level
from level import conc_ind
from level import level
import argparse
import cmudict
import data


dict_level = level[str, str, str]


class reversal:
    def __init__(self, phrase: str) -> None:
        self._database_root = data.ROOT
        self._database_dict = cmudict.dict()
        self._interchange_t = data.INTERCHANGES
        self._word_branches = dict[tuple[str, ...], frame_level]()
        self._char_branches = dict[tuple[str, ...], frame_level]()
        self.phrase = phrase.lower().split(' ')

        self.root = frame_level()
        for path in self.__get_pronunciations():
            self.__fill(path, self.root, self._database_root)
        # self.root.clean()

    def __get_pronunciations(self) -> list[tuple]:
        options = [()]
        database = cmudict.dict()
        for w in self.phrase:
            options = [
                (*reversed(t), *v)
                for v in options
                for t in database[w]
            ]
        return options

    def __char(
        self,
        frame: frame_level,
        path: tuple[str, ...],
        path_snip: tuple[str, ...],
        new_scores: tuple[int, ...],
        w_br: dict_level,
    ) -> None:
        conc_snip = tuple(
            conc_ind(value=c, score=s)
            for c, s in zip(path_snip, new_scores)
        )

        frame_new_brs = frame.new_branch(conc_snip)
        self.__fill(path, frame_new_brs[-1], w_br)

    def __word(
        self,
        frame: frame_level,
        path: tuple[str, ...],
        words: dict_level,
    ) -> None:
        frame.items.update(words.items)
        if len(frame.items) == 0:
            return

        saved_branch = self._word_branches.get(path, None)
        if not saved_branch:
            saved_branch = self._word_branches[path] = frame_level()
            self.__fill(path, saved_branch, self._database_root)
        frame.add_branch((conc_ind(),), saved_branch)

    def __fill(
        self,
        path: tuple[str],
        frame: frame_level,
        words: dict_level,
    ) -> None:
        if len(path) == 0:
            frame.items.update(words.items)
            return

        if not frame or not words:
            return

        options = [
            (v, path[i:], s)
            for i in range(1, len(path) + 1)
            for v, s in self._interchange_t.get(path[:i], {}).items()
        ]

        for path_snip, new_path, new_scores in options:
            w_br = words.get(path_snip)
            if not w_br:
                continue

            self.__word(frame, new_path, words)
            self.__char(frame, new_path, path_snip, new_scores, w_br)

    def print_tree(self) -> None:
        pairs = self.root.word_pairs()
        pairs.sort()
        print('\n'.join([
            f'{sre:3d} - {phr}'
            for sre, phr in pairs
        ]))


if __name__ == '__main__':
    args = argparse.ArgumentParser()
    args.add_argument('phrase', type=str)
    parsed = args.parse_args()

    rev = reversal(parsed.phrase)
    rev.print_tree()
