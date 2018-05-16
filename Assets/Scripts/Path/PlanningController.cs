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
        if (!pathDrawers.ContainsKey(startCell))
            pathDrawers.Add(startCell, PathManager.CreatePathDrawer());
        return pathDrawers[startCell];
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
                GetPathDrawer(startCell).DrawPath(path);
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
    }

    void BuildPath()
    {
    }
}
