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
        if (Input.GetButtonUp("Fire1") && targetCell != GridManager.GetMouseCell())
        {
            targetCell = GridManager.GetMouseCell();
            BuildPath();
        }
	}

    void BuildPath()
    {
        tilemap.ClearAllTiles();
        Cell min = new Cell(int.MaxValue, int.MaxValue), max = new Cell(-int.MaxValue, -int.MaxValue);
        min.x = ((startCell.x < targetCell.x) ? startCell.x : targetCell.x);
        min.y = ((startCell.y < targetCell.y) ? startCell.y : targetCell.y);
        max.x = ((startCell.x > targetCell.x) ? startCell.x : targetCell.x);
        max.y = ((startCell.y > targetCell.y) ? startCell.y : targetCell.y);

        DataGrid<bool> availability = new DataGrid<bool>(min, max);
        for (int i = min.x; i <= max.x; i++)
        {
            for (int j = min.y; j <= max.y; j++)
            {
                availability[new Cell(i, j)] = true;
            }
            availability[new Cell(0, 0)] = false;

        }
        /*for (int i = 0; i < path.Count; i++)
        {
            if (path[i].x < min.x)
                min.x = path[i].x;
            if (path[i].y < min.y)
                min.y = path[i].y;
            if (path[i].x > max.x)
                max.x = path[i].x;
            if (path[i].y > max.y)
                max.y = path[i].y;
        }*/
        DataGrid<Pathfind.CellData> dijkstra = Pathfind.Dijkstra(startCell, availability);
        List<Cell> path = Pathfind.GetPathFromDijkstra(dijkstra, startCell, targetCell);
        Cell size = new Cell(max.x - min.x, max.y - min.y);
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
