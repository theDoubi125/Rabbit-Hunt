using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.Tilemaps;

[RequireComponent(typeof(Tilemap))]
public class PlanningController : MonoBehaviour
{
    public List<Tile> Tiles;

    private bool selection = false;
    private Cell startCell, targetCell;

    private Tilemap tilemap;
    public Tilemap collisionTilemap;

    void Start()
    {
        tilemap = GetComponent<Tilemap>();
    }
	
	void Update ()
    {
        if (Input.GetButtonDown("Fire1"))
        {
            startCell = GridManager.GetMouseCell();
            tilemap.ClearAllTiles();
        }
        if (Input.GetButtonUp("Fire1"))
        {
            targetCell = GridManager.GetMouseCell();
            BuildPath();
        }
	}

    void BuildPath()
    {
        tilemap.ClearAllTiles();
        DataGrid<Pathfind.CellData> dijkstra = Pathfind.Dijkstra(startCell, collisionTilemap);
        List<Cell> path = Pathfind.GetPathFromDijkstra(dijkstra, startCell, targetCell);
        Cell min = new Cell(int.MaxValue, int.MaxValue), max = new Cell(-int.MaxValue, -int.MaxValue);
        foreach (Cell cell in path)
        {
            if(min.x > cell.x)
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
