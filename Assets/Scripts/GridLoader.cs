using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.Tilemaps;

[RequireComponent(typeof(GridManager))]
public class GridLoader : MonoBehaviour
{
    public GridData data;
    public Tilemap tileMap;

    public Transform wolfPrefab;
    public Transform bunnyPrefab;

    public void Start()
    {
        GridManager gridManager = GetComponent<GridManager>();
        for(int i=0; i<data.w; i++)
        {
            for(int j=0; j<data.h; j++)
            {
                TileBase tile = data.GetTileAt(new Cell(i, j));
                tileMap.SetTile(new Vector3Int(i, j, 0), tile);
            }
        }

        for (int i = 0; i < data.rabbits.Count; i++)
        {
            Transform rabbit = Instantiate<Transform>(bunnyPrefab, GridManager.GetCellPos(data.rabbits[i]), Quaternion.identity);
            GridManager.RegisterEntity(rabbit);
        }

        for (int i = 0; i < data.wolves.Count; i++)
        {
            Transform wolf = Instantiate<Transform>(wolfPrefab, GridManager.GetCellPos(data.wolves[i]), Quaternion.identity);
            GridManager.RegisterEntity(wolf);
        }
    }
}

[CreateAssetMenu(fileName = "Grid Data", menuName = "Grid/GridData", order = 1)]
public class GridData : ScriptableObject
{
    public int w, h;
    public List<TileBase> tiles = new List<TileBase>();
    public List<Cell> wolves;
    public List<Cell> rabbits;

    public TileBase GetTileAt(Cell cell)
    {
        if(cell.x >= 0 && cell.x < w && cell.y >= 0 && cell.y < h)
            return tiles[cell.x + cell.y * w];
        return null;
    }
}

[CreateAssetMenu(fileName = "Game Tile", menuName = "Grid/Tile", order = 1)]
public class GameTile : Tile
{
    public int collisionType = 0;
}