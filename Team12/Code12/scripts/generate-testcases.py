#!/usr/bin/env python3

import itertools


######################
# Edit here only
######################
SUCH_THAT_COUNT = 2
WITH_COUNT = 0  # TODO: not yet implemented
PATTERN_COUNT = 0  # TODO: not yet implemented

# additional possible values for the entities
# keys refer to all_design_entities below
# values are either integer or strong
actual_substitute = {
    "stmt": 1,
    "variable": "a",
}

# comment out relationships that you don't want to include
all_relationships = [
    "Uses",
    "Uses*",
    "Modifies",
    "Modifies*",
    "Parent",
    "Parent*",
    "Follows",
    "Follows*",
    # "Next",
    # "Next*",
    # "Calls",
    # "Calls*",
    # "Affects",
    # "Affects*"
]

# Only select synonyms that are used in clauses
# Recommended: True. False will increase by A LOT
SELECT_RELATED = True
# Allow permutation of such that clauses
# Recommended: False. True will increase by A LOT
ALLOW_CLAUSE_PERM = False
# Force all such that clauses to share at least one
# synonym with at least one other clause
# Recommended: True
FORCE_SHARE_SYNONYM = True  # TODO: implement

######################
# Do not edit beyond here
######################

clause_count = {
    "such that": SUCH_THAT_COUNT,
    "with": WITH_COUNT,
    "pattern": PATTERN_COUNT,
}

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
stmt_entities = ["stmt", "if", "while", "assign", "print", "read", "call"]
possible_entities = {
    "Uses": [uses_entities, ["variable"]],
    "Uses*": [uses_entities, ["variable"]],
    "Modifies": [modifies_entities, ["variable"]],
    "Modifies*": [modifies_entities, ["variable"]],
    "Follows": [stmt_entities, stmt_entities],
    "Follows*": [stmt_entities, stmt_entities],
    "Parent": [["if", "while"], stmt_entities],
    "Parent*": [["if", "while"], stmt_entities],
    "Next": [stmt_entities, stmt_entities],
    "Next*": [stmt_entities, stmt_entities],
    "Calls": [["procedure"], ["procedure"]],
    "Calls*": [["procedure"], ["procedure"]],
    "Affects": [["assign"], ["assign"]],
    "Affects*": [["assign"], ["assign"]],
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
    return {
        entity: [entity[: min(len(entity), 3)] + str(i) for i in range(count[entity])]
        for entity in count
    }


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


def quote_actual(val):
    """
    If val is integer, return str(val). Else if it is string,
    return it padded with double quotes.

    @param val string or integer
    """
    return str(val) if type(val) == int else f'"{val}"'


def unquote_actual(val):
    """
    If val is integer, return str(val). Else if it is string,
    return it padded with double quotes.

    @param val a string
    """
    return val.replace('"', "") if '"' in val else int(val)


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
            # if there a concrete value, add it into poss
            if entity in actual_substitute:
                # given variable "a", it must be '"a"' in PQL
                poss.append(quote_actual(actual_substitute[entity]))

        ordered_possibilities.append(poss)


def print_such_that_clause(clause, left_synonym, right_synonym):
    """
    Print out "Next*(a, b)"

    @param clause eg. "Next*"
    @param left_synonym, eg. stmt1
    @param right_synonym, eg. stmt1
    """
    print("{}({}, {})".format(clause, left_synonym, right_synonym), end=" ")


def print_such_that(clauses, synonyms, num_clause):
    """
    Print out "Such that ..."

    @param clauses iterable of ["Next*"] etc
    @param synonyms iterable of ["stmt2", "assign3"] etc
    @param num_clause number of clauses, equivalent to len(clauses)
    """
    print("such that", end=" ")
    for i in range(num_clause):
        print_such_that_clause(clauses[i], synonyms[2 * i], synonyms[2 * i + 1])


# TODO: generate pattern, with as well
def generate(
    clause_count=clause_count,
    select_related=SELECT_RELATED,
    allow_clause_perm=ALLOW_CLAUSE_PERM,
):
    """
    Main method that generates and prints all queries according to clause_count.

    There are currently 3 layers of permutations:
    1. combination of such that relationships (Next, Follows etc)
    2. combination of synonyms across all clauses
    3. choice of "Select"ed synonym
    and are represented by 3 nested for loops

    @param clause_count a dict with keys "such that", "pattern" and "with" to integers
    """
    # If there are k clauses, there can be maximally 2k different synonyms for one entity.
    count_for_decl = {
        entity: 2 * sum(clause_count.values()) for entity in all_design_entities
    }
    entity_to_synonyms_map = generate_entity_to_synonyms_map(count_for_decl)
    decl = generate_declaration(entity_to_synonyms_map)
    all_synonyms = list(itertools.chain.from_iterable(entity_to_synonyms_map.values()))
    cannot_select = [quote_actual(val) for val in actual_substitute.values()]
    # Layer 1
    # combinations allows permutations
    for clause_config in (
        itertools.product(all_relationships, repeat=clause_count["such that"])
        if allow_clause_perm
        else itertools.combinations_with_replacement(
            all_relationships, r=clause_count["such that"]
        )
    ):
        count = {entity: 0 for entity in all_design_entities}
        for clause in clause_config:
            update_count(clause, count)
        # print(count)
        entity_to_synonyms_map = generate_entity_to_synonyms_map(count)
        # ordered_possibilities contains as many elements as there are blanks,
        # where each relationship (eg Next(_,_)) has two blanks.
        # each element is a list containing possibilities for each blank, in order.
        # we loop over each clause to generate all possibilities.
        ordered_possibilities = []
        for clause in clause_config:
            generate_possibilities(
                clause, entity_to_synonyms_map, ordered_possibilities
            )
        # Layer 2: synonym permutations.
        # TODO: change product to something else for FORCE_SHARE_SYNONYM
        for synonym_config in itertools.product(*ordered_possibilities):
            num_clause = clause_count["such that"]
            # Layer 3: choice of selected synonym
            # change all_synonyms to synonym_config for relevant result
            for selected in set(synonym_config) if select_related else all_synonyms:
                # do not select explicit values
                if selected in cannot_select:
                    continue
                print(decl, end=" ")
                print(f"Select {selected}", end=" ")
                print_such_that(clause_config, synonym_config, num_clause)
                print()  # separate each query with one newline


generate()
