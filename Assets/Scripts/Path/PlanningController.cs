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
    
    public Tilemap collisionTilemap;

    public Transform startCursorPrefab, endCursorPrefab;
    private Transform startCursorInstance, endCursorInstance;

    Dictionary<Cell, PathDrawer> pathDrawers = new Dictionary<Cell, PathDrawer>();
    Dictionary<Cell, List<Cell>> paths = new Dictionary<Cell, List<Cell>>();
    PathDrawer previewPathDrawer;

    private bool m_characterSelected;

    void Start()
    {
        if (startCursorPrefab != null)
            startCursorInstance = Instantiate<Transform>(startCursorPrefab, GridManager.GetCellPos(startCell), Quaternion.identity);
        if (endCursorPrefab != null)
            endCursorInstance = Instantiate<Transform>(endCursorPrefab, GridManager.GetCellPos(targetCell), Quaternion.identity);
        previewPathDrawer = PathManager.CreatePathDrawer();
    }

    private PathDrawer GetPathDrawer(Cell cell)
    {
        if (!pathDrawers.ContainsKey(cell))
            pathDrawers.Add(cell, PathManager.CreatePathDrawer());
        return pathDrawers[cell];
    }

    private void SetPathForCell(Cell cell, List<Cell> path)
    {
        if (!pathDrawers.ContainsKey(cell))
            pathDrawers.Add(startCell, PathManager.CreatePathDrawer());
        if (!paths.ContainsKey(cell))
            paths.Add(cell, path);
        
        pathDrawers[cell].DrawPath(path);
        paths[cell] = path;    
    }
	
	void Update ()
    {
        Cell selectedCell = GridManager.GetMouseCell();
        if (Input.GetButtonDown("Fire1"))
        {
            if (GridManager.GetEntityAt<Character>(selectedCell) != null)
            {
                startCell = GridManager.GetMouseCell();
                if (startCursorInstance != null)
                    startCursorInstance.transform.position = GridManager.GetCellPos(startCell);

                previewPathDrawer.ClearPath();
            }
            else
            {
                List<Cell> path = Pathfind.GetPathFromDijkstra(Pathfind.Dijkstra(startCell, collisionTilemap), startCell, targetCell);
                SetPathForCell(startCell, path);
            }
        }
        if(targetCell != GridManager.GetMouseCell())
        {
            targetCell = GridManager.GetMouseCell();
            if (endCursorInstance != null)
                endCursorInstance.transform.position = GridManager.GetCellPos(targetCell);
            List<Cell> path = Pathfind.GetPathFromDijkstra(Pathfind.Dijkstra(startCell, collisionTilemap), startCell, targetCell);
            previewPathDrawer.DrawPath(path);
        }
        if (Input.GetButtonDown("Fire2"))
        {
            LaunchSimulation();
        }
    }

    void BuildPath()
    {
    }

    void LaunchSimulation()
    {
        foreach (Cell cell in paths.Keys)
        {
            CharacterSimulator character = GridManager.GetEntityAt<CharacterSimulator>(cell);
            if (character != null)
            {
                character.StartSimulation(paths[cell]);
            }
        }
        ClearAllPaths();
    }

    void ClearAllPaths()
    {
        foreach (Cell cell in pathDrawers.Keys)
        {
            pathDrawers[cell].ClearPath();
            PathManager.ReleasePathDrawer(pathDrawers[cell]);
        }

        pathDrawers.Clear();
        paths.Clear();
    }
}
