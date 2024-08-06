import folium
import json
import os
import webbrowser

print("Hello, Python! Creating and Loading Web Map")

read = open("output/data.json")
graphData = open("output/graph.json")

data = json.load(read)
meta = data["metadata"]

x = meta["lat"]
y = meta["long"]
region = meta["region"]

m = folium.Map(location = [x, y], zoom_start = 1)
m.save("output/map.html")

read.close()