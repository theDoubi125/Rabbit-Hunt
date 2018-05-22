using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CharacterSimulator : MonoBehaviour
{
    private Cell startCell;
    private List<Cell> path = new List<Cell>();
    private float time = 0;

    public void StartSimulation(List<Cell> inPath)
    {
        startCell = GridManager.GetCellAt(transform.position);
        path.Add(startCell);
        path.AddRange(inPath);
    }

    void FixedUpdate()
    {
        time += Time.fixedDeltaTime;
        if (time > 1)
        {
            time -= 1;

            if (path.Count > 2)
                path.RemoveAt(0);
            else
            {
                path.Clear();
                GridManager.UpdateEntityCell(transform);
            }
        }
        if (path.Count >= 2)
        {
            Vector3 A = GridManager.GetCellPos(path[0]);
            Vector3 B = GridManager.GetCellPos(path[1]);
            transform.position = A + (B - A) * time;
        }
    }
}
