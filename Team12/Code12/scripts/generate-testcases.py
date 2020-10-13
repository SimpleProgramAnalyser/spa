#!/usr/bin/env python3

import itertools

SUCH_THAT_COUNT = 2
WITH_COUNT = 0
PATTERN_COUNT = 0

clause_count = {
    "such that": SUCH_THAT_COUNT,
    "with": WITH_COUNT,
    "pattern": PATTERN_COUNT,
}

# TODO: all relationships possible for such that
all_relationships = ["Uses", "Uses*", "Modifies", "Modifies*"]
all_design_entities = [
    "stmt",
    "if",
    "while",
    "assign",
    "print",
    "read",
    "call",
    "procedure",
    "variable",
    "constant",
]
uses_entities = ["stmt", "if", "while", "assign", "print", "call", "procedure"]
modifies_entities = ["stmt", "if", "while", "assign", "read", "call", "procedure"]
possible_entities = {
    "Uses": [uses_entities, ["variable"]],
    "Uses*": [uses_entities, ["variable"]],
    "Modifies": [modifies_entities, ["variable"]],
    "Modifies*": [modifies_entities, ["variable"]],
}


def update_count(clause, count):
    """
    @param clause: a string like "Next*"
    @param count: a dict like {"stmt": 3, "procedure": 0}

    @return void
    """
    entity_groups = possible_entities[clause]
    for entity_group in entity_groups:
        for entity in entity_group:
            # print(count)
            count[entity] += 1


def generate_entity_to_synonyms_map(count):
    """
    Generate a dict like {"stmt": ["stmt1", "stmt2", "stmt3"]}
    @param count: a dict like {"stmt": 3, "procedure": 0}

    @return a dict like {"stmt": ["stmt1", "stmt2", "stmt3"]}
    """
    return {entity: [entity + str(i) for i in range(count[entity])] for entity in count}


def generate_declaration(entity_to_synonyms_map):
    """
    Generate "stmt stmt1, stmt2, stmt3;..."
    @param entity_to_synonyms_map: a dict like {"stmt": ["stmt1", "stmt2", "stmt3"]}

    @return a PQL declaration string
    """
    declarations = [
        "{} {}; ".format(entity, ", ".join(entity_to_synonyms_map[entity]))
        for entity in entity_to_synonyms_map
        if entity_to_synonyms_map[entity]
    ]
    return "".join(declarations)


def generate_possibilities(clause, entity_to_synonyms_map, ordered_possibilities):
    """
    @param clause: a string like "Next*"
    @param entity_to_synonyms_map: a dict like {"stmt": ["stmt1", "stmt2", "stmt3"]}
    @param ordered_possibilities a list of lists of possibilities

    @return void
    """
    for i in range(2):  # left, right
        poss = []
        for entity in possible_entities[clause][i]:
            poss.extend(entity_to_synonyms_map[entity])
        ordered_possibilities.append(poss)


def print_such_that_clause(clause, left_synonym, right_synonym):
    """
    Print out "Next*(a, b)"

    @param clause eg. "Next*"
    @param left_synonym, eg. stmt1
    @param right_synonym, eg. stmt1
    """
    print("{}({}, {})".format(clause, left_synonym, right_synonym), end=" ")


def print_such_that(clauses, synonyms):
    """
    Print out "Such that ..."

    @param clauses iterable of ["Next*"] etc
    @param synonyms iterable of ["stmt2", "assign3"] etc
    """
    print("such that", end=" ")
    for i in range(len(clauses)):
        print_such_that_clause(clauses[i], synonyms[2 * i], synonyms[2 * i + 1])


def generate(clause_count=clause_count):
    # Change to itertools.combination to test with permutation
    for clause_config in itertools.combinations_with_replacement(
        all_relationships, r=clause_count["such that"]
    ):
        # we collate how many synonyms are there for each entity and
        # save it inside count. This will be used to
        # 1. generate the declaration
        # 2. generate all synonym permutations
        count = {entity: 0 for entity in all_design_entities}
        # print(count)
        for clause in clause_config:
            update_count(clause, count)
        entity_to_synonyms_map = generate_entity_to_synonyms_map(count)
        decl = generate_declaration(entity_to_synonyms_map)
        # ordered_possibilities contains as many elements as there are blanks,
        # where each relationship (eg Next(_,_)) has two blanks.
        # each element is a list containing possibilities for each blank, in order.
        # we loop over each clause to generate all possibilities.
        ordered_possibilities = []
        for clause in clause_config:
            generate_possibilities(
                clause, entity_to_synonyms_map, ordered_possibilities
            )
        all_synonyms = itertools.chain(*entity_to_synonyms_map.values())
        # print(list(all_synonyms))
        # finally, for this given clause_config, we print out all the PQL query,
        # subjected to synonym permutations.
        for synonym_config in itertools.product(*ordered_possibilities):
            # loop through 2 at once
            num_synonym = len(synonym_config)
            num_clause = clause_count["such that"]
            assert num_clause * 2 == num_synonym
            for selected in all_synonyms:
                print(decl, end=" ")  # FIXME
                print(f"Select {selected}", end=" ")
                print_such_that(clause_config, synonym_config)
                print()  # separate each query with one newline


generate()
