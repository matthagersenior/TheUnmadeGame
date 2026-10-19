"""Unreal Editor-only, idempotent authoring of a tintable native prototype material.

Run inside an Unreal Editor Python session (Python Editor Script Plugin enabled).
For the first Windows Unreal PC:
    UnrealEditor-Cmd.exe TheUnmadeGame.uproject -run=pythonscript -script=Scripts/unreal/author_realm_palette.py

This script is intentionally NOT invoked by ordinary Linux source CI.
It creates only /Game/Art/Generated/M_UnmadeRealmPalette. The renderer's
UnmadePrototypeHub automatically uses it if the cooked material exists,
or falls back to Engine BasicShapeMaterial if it does not.
"""
import unreal

ASSET_DIRECTORY = "/Game/Art/Generated"
ASSET = ASSET_DIRECTORY + "/M_UnmadeRealmPalette"


def author():
    assets = unreal.EditorAssetLibrary
    if not assets.does_directory_exist(ASSET_DIRECTORY):
        if not assets.make_directory(ASSET_DIRECTORY):
            raise RuntimeError("Cannot create " + ASSET_DIRECTORY)

    existing = assets.load_asset(ASSET)
    if existing is not None:
        if not isinstance(existing, unreal.Material):
            raise TypeError(ASSET + " exists but is not a Material")
        material = existing
    else:
        material = unreal.AssetToolsHelpers.get_asset_tools().create_asset(
            "M_UnmadeRealmPalette",
            ASSET_DIRECTORY,
            unreal.Material,
            unreal.MaterialFactoryNew(),
        )
        if material is None:
            raise RuntimeError("Could not create master material")

    # Safe to rerun: rebuild just this generator-owned graph.
    editing = unreal.MaterialEditingLibrary
    editing.delete_all_material_expressions(material)

    color = editing.create_material_expression(
        material, unreal.MaterialExpressionVectorParameter, -410, 0
    )
    color.set_editor_property("parameter_name", "Color")
    color.set_editor_property(
        "default_value", unreal.LinearColor(0.28, 0.30, 0.34, 1.0)
    )
    assert editing.connect_material_property(
        color, "", unreal.MaterialProperty.MP_BASE_COLOR
    ), "Color must be linked to Base Color"

    rough = editing.create_material_expression(
        material, unreal.MaterialExpressionScalarParameter, -410, 180
    )
    rough.set_editor_property("parameter_name", "Roughness")
    rough.set_editor_property("default_value", 0.72)
    assert editing.connect_material_property(
        rough, "", unreal.MaterialProperty.MP_ROUGHNESS
    ), "Roughness must be linked to the roughness property"

    editing.recompile_material(material)
    if not assets.save_loaded_asset(material, only_if_is_dirty=False):
        raise RuntimeError("Could not save " + ASSET)
    if not assets.does_asset_exist(ASSET):
        raise RuntimeError("Material was not saved into the Content directory")
    unreal.log("The Unmade: authored cookable master palette with Color parameter: " + ASSET)


if __name__ == "__main__":
    author()
