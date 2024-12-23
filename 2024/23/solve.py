from collections import defaultdict
from itertools import combinations


class Graph:
    def __init__(self):
        self.nodes = set()
        self.edges = set()
        self.adjacent = defaultdict(set)

    def from_edges(self, edges):
        for edge in edges:
            self.edges.add(edge)
            self.nodes.add(edge[0])
            self.nodes.add(edge[1])
            self.adjacent[edge[0]].add(edge[1])
            self.adjacent[edge[1]].add(edge[0])

        return self

    def find_cliques(self, k):
        cliques = [set(edge) for edge in self.edges]
        n = 2
        while cliques and n < k:
            temp = set()
            for u, v in combinations(cliques, 2):
                w = u ^ v
                if len(w) == 2:
                    m = tuple(w)
                    if m[0] in self.adjacent[m[1]]:
                        temp.add(tuple(u | w))

            cliques = set(map(tuple, map(sorted, temp)))
            n += 1

        return cliques

    def max_clique(self):
        found = []

        def bron_kerbosch(r, p, x):
            if len(p) == 0 and len(x) == 0:
                if len(r) > 2:
                    found.append(sorted(r))

                return

            for v in p.union(set([])):
                bron_kerbosch(
                    r.union(set([v])),
                    p.intersection(self.adjacent[v]),
                    x.intersection(self.adjacent[v]),
                )
                p.remove(v)
                x.add(v)

        bron_kerbosch(set([]), self.nodes, set([]))
        return found


edges = list(map(lambda x: tuple(x.split("-")), open("input.txt").read().split("\n")))
g = Graph().from_edges(edges)

cliques = g.find_cliques(3)
potentials = sum(map(lambda x: any(n.startswith("t") for n in x), cliques))
print(f"Part 1: {potentials}")

maxclique = g.max_clique()
password = ",".join(max(maxclique, key=len))
print(f"Part 2: {password}")
