using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.Tilemaps;

public class PathDrawer : MonoBehaviour
{
    public List<Tile> Tiles;

    private bool selection = false;
    private Cell startCell, targetCell;

    private Tilemap m_tilemap;
    private Tilemap tilemap
    {
        get
        {
            if (m_tilemap == null)
                m_tilemap = GetComponentInChildren<Tilemap>();
            return m_tilemap;
        }
    }
    public Tilemap collisionTilemap;

    void Start()
    {

    }

    public void ClearPath()
    {
        tilemap.ClearAllTiles();
    }

    public void DrawPath(List<Cell> path)
    {
        tilemap.ClearAllTiles();
        DataGrid<Pathfind.CellData> dijkstra = Pathfind.Dijkstra(startCell, collisionTilemap);
        Cell min = new Cell(int.MaxValue, int.MaxValue), max = new Cell(-int.MaxValue, -int.MaxValue);
        foreach (Cell cell in path)
        {
            if (min.x > cell.x)
                min.x = cell.x;
            if (min.y > cell.y)
                min.y = cell.y;
            if (max.x < cell.x)
                max.x = cell.x;
            if (max.y < cell.y)
                max.y = cell.y;
        }
        DataGrid<int> grid = new DataGrid<int>(min, max);

        foreach (Cell cell in path)
        {
            grid[cell + Cell.left] += 1 << 0;
            grid[cell + Cell.up] += 1 << 1;
            grid[cell + Cell.right] += 1 << 2;
            grid[cell + Cell.down] += 1 << 3;
        }
        foreach (Cell cell in path)
        {
            tilemap.SetTile(new Vector3Int(cell.x, cell.y, 0), Tiles[grid[cell]]);
        }
    }
}
