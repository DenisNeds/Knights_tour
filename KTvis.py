import numpy as np
import matplotlib.pyplot as plt

# bere podatke iz output_data.txt
with open("output_data.txt", "r") as file:
    lines = file.readlines()

data = [list(map(int, line.strip().split("\t"))) for line in lines]

data_array = np.array(data)

# mozni premiki
cx = [1, 1, 2, 2, -1, -1, -2, -2]
cy = [2, -2, 1, -1, 2, -2, 1, -1]

#crno belo ozadje
board_size_x = len(data_array[0])
board_size_y = len(data_array)
chessboard = np.zeros((board_size_y, board_size_x))
chessboard[1::2, ::2] = 1
chessboard[::2, 1::2] = 1

plt.figure(figsize=(20, 20))
plt.imshow(chessboard, cmap="binary", origin="upper")

for i in range(len(data_array)):
    for j in range(len(data_array[i])):
        cell_value = data_array[i, j]
        if cell_value == 1:  # First cell
            plt.scatter(j, i, s=150, color="green", marker="o", edgecolor="black")
        elif cell_value == board_size_x * board_size_y:  # Last cell
            plt.scatter(j, i, s=150, color="red", marker="o", edgecolor="black")
        plt.text(j, i, str(cell_value), ha="center", va="center", color="orange", fontsize=8)

for i in range(board_size_y):
    for j in range(board_size_x):
        current_number = data_array[i, j]
        if current_number != -1:
            next_numbers = [(i + cy[k], j + cx[k]) for k in range(8)]
            for next_i, next_j in next_numbers:
                if (
                    0 <= next_i < board_size_y
                    and 0 <= next_j < board_size_x
                    and data_array[next_i, next_j] == current_number + 1
                ):
                    plt.plot([j, next_j], [i, next_i], color="darkorange")

plt.title("Knight's tour")
plt.xlabel("stolpec")
plt.ylabel("vrstica")
plt.show()
