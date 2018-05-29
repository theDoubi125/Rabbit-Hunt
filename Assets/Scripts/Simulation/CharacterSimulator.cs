using System.Collections;
using System.Collections.Generic;
using UnityEngine;

[RequireComponent(typeof(Character))]
public class CharacterSimulator : MonoBehaviour
{
    private Cell startCell;
    private List<Cell> path = new List<Cell>();
    private float time = 0;
    private Character character;

    public delegate void CharacterSimulationFinishedDelegate(CharacterSimulator character);

    public CharacterSimulationFinishedDelegate OnSimulationFinished;

    private void Start()
    {
        character = GetComponent<Character>();
    }

    public void StartSimulation(List<Cell> inPath)
    {
        startCell = GridManager.GetCellAt(transform.position);
        path.AddRange(inPath);
        enabled = true;
    }

    void FixedUpdate()
    {
        time += Time.fixedDeltaTime * (character != null ? character.velocity : 1);
        if (time > 1)
        {
            time -= 1;

            if (path.Count > 2)
                path.RemoveAt(0);
            else
            {
                path.Clear();
                GridManager.UpdateEntityCell(transform);
                if(OnSimulationFinished != null)
                    OnSimulationFinished(this);
                enabled = false;
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
