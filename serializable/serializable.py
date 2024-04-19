from typing import List, Tuple

data_items = dict()
conflict_graph = dict()

class DataItem:
    last_writer: str | None = None
    last_readers: List[str] = []


def parse_operations(input_string: str):
    tokens = input_string.split(' ')
    return [(tokens[i], tokens[i+1], tokens[i+2]) for i in range(0, len(tokens), 3)]


def init_data_structures(operations: List[Tuple[str, str, str]]):
    for (_, transaction_id, data_item_id) in operations:
        if not transaction_id in conflict_graph:
            conflict_graph[transaction_id] = []
        if not data_item_id in data_items:
            data_items[data_item_id] = DataItem()

# TODO: Do not add reflexive cycles


def build_conflict_graph(operations: List[Tuple[str, str, str]]):
    for (op, transaction_id, data_item_id) in operations:
        
        if op == 'w':
            # All reads before this operation conflict with the current write
            for reader in data_items[data_item_id].last_readers:
                conflict_graph[reader] += transaction_id
            
            # The last write before this operation conflicts with the current write (transitive closure omitted)
            last_writer = data_items[data_item_id].last_writer
            if last_writer is not None:
                conflict_graph[last_writer] += transaction_id

            # Update last writer as the current transaction
            data_items[data_item_id].last_writer = transaction_id

            # Clear the list of last readers
            data_items[data_item_id].last_readers = []

        if op == 'r':
            # Add the current transaction to the last readers list
            data_items[data_item_id].last_readers += transaction_id
            last_writer = data_items[data_item_id].last_writer
            if last_writer is not None:
                print(f'last writer was {last_writer} which conflicted with {transaction_id}')
                conflict_graph[last_writer] += transaction_id
        


operations = parse_operations(input())
init_data_structures(operations)
build_conflict_graph(operations)

print(operations)

print('----')

for x in conflict_graph.items():
    print (x)

print("---")

for (k ,v) in data_items.items():
    print (k, v.last_writer, v.last_readers)