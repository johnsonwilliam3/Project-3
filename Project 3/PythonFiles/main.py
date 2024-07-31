import folium

print("Hello, Python! Creating Web Map")
m = folium.Map()
m.save("/output/footprint.html")