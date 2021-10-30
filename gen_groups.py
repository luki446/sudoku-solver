groups = []

for i in range(9):
    groups.append([])

for x in range(9):
    for y in range(9):
        if 0 <= x <= 2:
            x_index = 0
        elif 3 <= x <= 5:
            x_index = 1
        else:
            x_index = 2

        if 0 <= y <= 2:
            y_index = 0
        elif 3 <= y <= 5:
            y_index = 1
        else:
            y_index = 2

        groups[x_index * 3 + y_index].append((x, y))

for group in groups:
    ctx = ", ".join([f"calc_index({coords[0]}, {coords[1]})" for coords in group ])
    print(f"\t{{ {ctx} }},")

# print(groups)
