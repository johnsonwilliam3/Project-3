import folium
import json
import webbrowser

print("Hello, Python! Creating Web Map")
m = folium.Map(location = [31.9, -93.4], zoom_start = 16)
m.save("output/footprint.html")