import heapq
import tkinter as tk
from tkinter import messagebox
import matplotlib.pyplot as plt

class CityMap:
    def __init__(self):
        self.graph = {}
        self.heuristic = {}
        self.positions = {}

    def add_location(self, loc, pos):
        self.graph[loc] = {}
        self.positions[loc] = pos

    def add_road(self, a, b, cost):
        self.graph[a][b] = cost
        self.graph[b][a] = cost

    def set_heuristic(self, h):
        self.heuristic = h

class AStar:
    def __init__(self, city):
        self.city = city

    def search(self, start, goal):
        open_list = []
        heapq.heappush(open_list, (0, start))

        g = {node: float('inf') for node in self.city.graph}
        g[start] = 0
        parent = {start: None}

        while open_list:
            _, current = heapq.heappop(open_list)

            if current == goal:
                return self.build_path(parent, current)

            for neighbor in self.city.graph[current]:
                cost = g[current] + self.city.graph[current][neighbor]

                if cost < g[neighbor]:
                    g[neighbor] = cost
                    f = cost + self.city.heuristic.get(neighbor, 0)
                    heapq.heappush(open_list, (f, neighbor))
                    parent[neighbor] = current

        return None

    def build_path(self, parent, node):
        path = []
        while node:
            path.append(node)
            node = parent[node]
        return path[::-1]

class DeliverySystem:
    def __init__(self, city):
        self.city = city
        self.astar = AStar(city)

    def multi_delivery(self, start, stops):
        current = start
        route = [start]

        for stop in stops:
            path = self.astar.search(current, stop)
            if path:
                route.extend(path[1:])
                current = stop
        return route

    def calculate_cost(self, route):
        cost = 0
        for i in range(len(route)-1):
            cost += self.city.graph[route[i]][route[i+1]]
        return cost

def draw_graph(city, route=None):
    plt.figure()

    for node in city.graph:
        x, y = city.positions[node]
        plt.scatter(x, y)
        plt.text(x, y+0.2, node)

        for neighbor in city.graph[node]:
            x2, y2 = city.positions[neighbor]
            plt.plot([x, x2], [y, y2])

    if route:
        for i in range(len(route)-1):
            x1, y1 = city.positions[route[i]]
            x2, y2 = city.positions[route[i+1]]
            plt.plot([x1, x2], [y1, y2], linewidth=3)

    plt.title("AI Delivery Route Optimization System")
    plt.show()

def setup():
    city = CityMap()

    city.add_location('A', (0,2))
    city.add_location('B', (2,3))
    city.add_location('C', (2,1))
    city.add_location('D', (4,2))
    city.add_location('E', (6,1))
    city.add_location('F', (6,3))

    city.add_road('A','B',4)
    city.add_road('A','C',2)
    city.add_road('B','D',5)
    city.add_road('C','D',8)
    city.add_road('C','E',10)
    city.add_road('D','F',6)
    city.add_road('E','F',3)

    city.set_heuristic({
        'A':10,'B':8,'C':5,'D':7,'E':3,'F':0
    })

    return city

city = setup()
system = DeliverySystem(city)

def find_path():
    start = start_entry.get().upper()
    goal = goal_entry.get().upper()

    if start not in city.graph or goal not in city.graph:
        messagebox.showerror("Error", "Invalid Input")
        return

    path = system.astar.search(start, goal)

    if path:
        cost = system.calculate_cost(path)
        result_var.set("Route: " + " -> ".join(path) + " | Cost: " + str(cost))
        draw_graph(city, path)
    else:
        result_var.set("No Path Found")

def multi_delivery():
    start = start_entry.get().upper()
    stops = stops_entry.get().upper().split(',')

    if start not in city.graph:
        messagebox.showerror("Error", "Invalid Start")
        return

    route = system.multi_delivery(start, stops)
    cost = system.calculate_cost(route)

    result_var.set("Route: " + " -> ".join(route) + " | Cost: " + str(cost))
    draw_graph(city, route)

root = tk.Tk()
root.title("AI Delivery Route Optimization System")
root.geometry("450x350")

tk.Label(root, text="Warehouse (Start)").pack(pady=5)
start_entry = tk.Entry(root)
start_entry.pack()

tk.Label(root, text="Delivery Location (Goal)").pack(pady=5)
goal_entry = tk.Entry(root)
goal_entry.pack()

tk.Button(root, text="Find Optimal Route", command=find_path).pack(pady=10)

tk.Label(root, text="Multiple Stops (A,B,C)").pack(pady=5)
stops_entry = tk.Entry(root)
stops_entry.pack()

tk.Button(root, text="Multi Delivery Route", command=multi_delivery).pack(pady=10)

result_var = tk.StringVar()
tk.Label(root, textvariable=result_var, fg="blue").pack(pady=10)

tk.Button(root, text="Show Map", command=lambda: draw_graph(city)).pack(pady=5)

root.mainloop()
