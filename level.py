import dataclasses
import functools
import typing


@dataclasses.dataclass(unsafe_hash=True, eq=True)
class conc_ind:
    score: int = 0
    value: str = ''


@dataclasses.dataclass(unsafe_hash=True, eq=True)
class dict_level:
    score: int = 0
    word: str = ''


ITEM_TYPE = typing.TypeVar('ITEM_TYPE')
PATH_TYPE = typing.TypeVar('PATH_TYPE')
RAW_PATH_TYPE = typing.TypeVar('RAW_PATH_TYPE')


class level(typing.Generic[ITEM_TYPE, PATH_TYPE, RAW_PATH_TYPE]):
    def __init__(self, stuff: dict[ITEM_TYPE, tuple[PATH_TYPE, ...]] = None) -> None:
        self.nexts = dict[RAW_PATH_TYPE, typing.Self]()
        self.items = set[ITEM_TYPE]()
        if stuff:
            for l in stuff:
                self.add_value(l)

    @functools.cache
    def branch_index(self, i: PATH_TYPE) -> RAW_PATH_TYPE:
        return i

    def add_value(self, t: tuple[PATH_TYPE, ...], s: ITEM_TYPE) -> None:
        nxts = self.new_branch(t) or (self,)
        nxts[-1].items.add(s)

    def add_branch(self, t: tuple[PATH_TYPE, ...], l: typing.Self) -> bool:
        nxts = self.new_branch(t[:-1]) or (self,)
        return nxts[-1].add_branch_index(t[-1], l)

    def add_branch_index(self, i: PATH_TYPE, l: typing.Self) -> bool:
        ind = self.branch_index(i)
        if ind in self.nexts:
            return False
        self.nexts[ind] = l
        return True

    def new_branch(self, t: tuple[PATH_TYPE, ...]) -> tuple[typing.Self, ...]:
        if len(t) == 0:
            return ()
        ind = self.branch_index(t[0])
        nxt = self.nexts.get(ind, None)
        if not nxt:
            nxt = self.__class__()
            self.add_branch_index(t[0], nxt)
        return (nxt, *nxt.new_branch(t[1:]))

    def get(self, t: tuple[PATH_TYPE, ...]) -> typing.Self | None:
        if len(t) == 0:
            return self
        return self.get_raw(tuple(
            self.branch_index(v)
            for v in t
        ))

    def get_raw(self, t: tuple[RAW_PATH_TYPE, ...]) -> typing.Self | None:
        if len(t) == 0:
            return self
        nxt = self.nexts.get(t[0], None)
        if not nxt:
            return None
        return nxt.get_raw(t[1:])

    def get_items(self, t: tuple[PATH_TYPE, ...]) -> list[ITEM_TYPE]:
        r = self.get(t)
        if not r:
            return set()
        return r.items

    def clean(self) -> bool:
        b = len(self.items) == 0
        for i, n in self.nexts.items():
            if n.clean():
                del self.nexts[i]
            else:
                b = False
        return b

    def values(self) -> list[ITEM_TYPE]:
        return [
            v
            for i, n in self.nodes()
            for v in n.items
        ]

    def entries(self) -> list[tuple[tuple[RAW_PATH_TYPE, ...], ITEM_TYPE]]:
        return [
            (i, v)
            for i, n in self.nodes()
            for v in n.items
        ]

    def nodes(self) -> list[tuple[tuple[RAW_PATH_TYPE, ...], typing.Self]]:
        return [
            ((), self),
        ] + [
            ((i, *t), nds)
            for i, n in self.nexts.items()
            for t, nds in n.nodes()
        ]


class frame_level(level[str, conc_ind, str]):
    def __init__(self) -> None:
        super().__init__()
        self.scres = dict[str, int]()

    @functools.cache
    def branch_index(self, i: conc_ind) -> str:
        return i.value

    def end_of_word(self) -> bool:
        return '' in self.nexts or len(self.nexts) == 0

    def end_of_phrase(self) -> bool:
        return len(self.nexts) == 0

    def add_branch_index(self, ind: conc_ind, l: typing.Self) -> bool:
        success = super().add_branch_index(ind, l)
        br_i = self.branch_index(ind)
        if br_i == '':
            return
        self.scres[br_i] = max(
            self.scres.get(br_i, -666),
            ind.score,
        )
        return success

    def word_pairs(self) -> list[tuple[int, str]]:
        if self.end_of_phrase():
            return [
                (0, w)
                for w in self.items
            ]

        items = list[tuple[str, typing.Self]](self.nexts.items())
        if self.end_of_word():
            return [
                (sre, f'{w_i} {w}')
                for i, n in items
                for sre, w in n.word_pairs()
                for w_i in self.items
            ]

        return [
            (sre + self.scres[i], w)
            for i, n in items
            for sre, w in n.word_pairs()
        ]
