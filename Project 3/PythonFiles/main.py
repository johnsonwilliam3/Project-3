import folium
import json
import webbrowser

print("Hello, Python! Creating Web Map")
m = folium.Map()
m.save("output/footprint.html")
webbrowser.open_new_tab("../output/footprint.html")