from level import level
import cmudict
import json


def expand_vowels(t: list[str]) -> list[list[str]]:
    r = [[]]
    for s in t:
        if s[0] in ['A', 'E', 'I', 'O', 'U']:
            v = ['0', '1', '2']
        else:
            v = ['']
        r = [
            [*rem, f'{s}{suf}']
            for rem in r
            for suf in v
        ]
    return r


def split_scores(d: dict[tuple[str, ...], int]) -> dict[tuple[str, ...], tuple[int, ...]]:
    res: dict[tuple[str, ...], tuple[int, ...]] = {}
    for t, s in sorted(d.items()):
        l = len(t)
        base = (
            *((l - 1) * [0]),
            s,
        )
        for splt in range(l - 1, 1, -1):
            t_part = t[:splt]
            if t_part in d:
                base = (
                    res[t_part],
                    *((l - splt - 1) * [0]),
                    s - d[t_part],
                )
                break
        res[t] = base
    return res


with open('data.json') as f:
    raw_data = json.load(f)

raw_interchanges: dict[str, dict[str, int]] = raw_data['INTERCHANGES']
inter_cum_scores: dict[tuple[str, ...], dict[tuple[str, ...], int]] = {}

pairs = [
    (i_e, v_e, s)
    for i, d in raw_interchanges.items()
    for v, s in d.items()
    for i_e in expand_vowels(i.split(' '))
    for v_e in expand_vowels(v.split(' '))
]

for i_t, v_t, score in pairs:
    inter_cum_scores.setdefault(
        (*reversed(i_t),), {}
    ).setdefault(
        (*reversed(v_t),), score
    )
    inter_cum_scores.setdefault(
        (*v_t,), {}
    ).setdefault(
        (*i_t,), score
    )

INTERCHANGES: dict[tuple[str, ...], dict[tuple[str, ...], tuple[int, ...]]] = {
    i: split_scores(d)
    for i, d in inter_cum_scores.items()
}


def make_root() -> level[str, str, str]:
    r = level[str, str, str]()
    e = cmudict.entries()
    for s, t in e:
        r.add_value(tuple(t), s)
    r.clean()
    return r


ROOT: level[str, str, str] = make_root()
