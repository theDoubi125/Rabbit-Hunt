using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.Tilemaps;

[RequireComponent(typeof(Tilemap))]
public class GridDisplay : MonoBehaviour
{
    public TileBase tile;
    private Tilemap tilemap;

    void OnEnable()
    {
        tilemap = GetComponent<Tilemap>();
    }

    public void ShowArea(List<Cell> cells)
    {
        foreach (Cell cell in cells)
        {
            tilemap.SetTile(new Vector3Int(cell.x, cell.y, 0), tile);
        }
    }
}
