import folium
import json
import pandas as pd
import os
import branca
import webbrowser

print("Hello, Python! Creating and Loading Web Map")

read = open("output/data.json")
data = json.load(read)
meta = data["metadata"]

x = meta["lat"]
y = meta["long"]
region = meta["region"]
use_centrality = meta["use_centrality"]

m = folium.Map([x, y], zoom_start = 4)
colors = ('#7d0000', '#ff0000', '#ff6a00','#fbff00', '#00ff2a', '#00ff84', '#00ffea', '#00d5ff', '#0080ff', '#001b91')

size = len(data["topCities"])
i = 0
col = 0
benchmark = size / 10
for city in data["topCities"]:
    lat = city["latitude"]
    lng = city["longitude"]
    city_name = city["city"]
    county = city["county"]
    state = city["state"]
    pop = city["population_rank"]
    congest = city["congestion_rank"]
    constr = city["construction_rank"]

    
    if i == benchmark:
        col += 1
        benchmark += size / 10

    if use_centrality == True:
        centrality = city["weighted_centrality"]
        html = """
        <h1> Data on {0}, {1} County, {2} </h1><br>
        <h3>
        Weighted_Centrality: {3:.5f} 
        </h3>
        <h4>
        Congestion Rank: {4} 
        </h4>
        <h4>
        Population Prediction Rank: {5} (Negative for decrease) 
        </h4>
        <h4>
        Construction Efficiency Rank: {6} 
        </h4>
        <h5>
        Latitude: {7:.2f} | Longitude: {8:.2f}
        """.format(city_name, county, state, centrality, congest, pop, constr, lat, lng)
    else:
        degree = city["weighted_degree"]
        html = """
        <h1> Data on {0}, {1} County, {2} </h1><br>
        <h3>
        Weighted_Centrality: {3:.5f} 
        </h3>
        <h4>
        Congestion Rank: {4} 
        </h4>
        <h4>
        Population Prediction Rank: {5} (Negative for decrease) 
        </h4>
        <h4>
        Construction Efficiency Rank: {6} 
        </h4>
        <h5>
        Latitude: {7:.2f} | Longitude: {8:.2f}
        """.format(city_name, county, state, degree, congest, pop, constr, lat, lng)

    folium.CircleMarker(
        location = [lat, lng],
        radius=20, 
        color='#000000',
        fill_color=colors[col],
        opacity = 1,
        popup=html).add_to(m)
    i += 1

m.save("output/map.html")

read.close()

webbrowser.open_new_tab(os.getcwd() + "/output/map.html")