from pathlib import Path
import xml.etree.ElementTree as ET

skin = Path(__file__).resolve().parents[1] / "snes9x-1.51-src-d3d/xbox/Skin"


def controls(name):
    return {
        node.findtext("Properties/Id"): node.find("Properties")
        for node in ET.parse(skin / name).iter()
        if node.findtext("Properties/Id")
    }


options = controls("InGameOptions.xui")
chain = ["XuiPointFiltering", "XuiMuteAudio", "XuiAspectRatio", "XuiFPS", "XuiFilterSimple2x"]
for first, second in zip(chain, chain[1:]):
    assert options[first].findtext("NavDown").split("\\")[-1] == second
    assert options[second].findtext("NavUp").split("\\")[-1] == first
for props in options.values():
    for direction in ("NavUp", "NavDown", "NavLeft", "NavRight", "DefaultFocus"):
        target = props.findtext(direction)
        if target:
            assert target.split("\\")[-1] in options, (direction, target)
assert options["XuiFPS"].findtext("NavRight") == "XuiMultitap"
assert options["XuiMultitap"].findtext("NavLeft") == "XuiFPS"
assert options["XuiMultitap"].findtext("NavDown") == "XuiButtonReset"
for scene in ("FavoritesListScene.xui", "Favorites.xui"):
    favorite = controls(scene)
    assert favorite["XuiRemoveFavorite"].findtext("NavRight") == "XuiMainMenu"
    assert favorite["XuiMainMenu"].findtext("NavLeft") == "XuiRemoveFavorite"
    assert favorite["XuiPlay"].findtext("NavRight") == "XuiRemoveFavorite"
print("UI navigation checks passed: FPS, multitap, Reset Game and Favorites removal are reachable.")
