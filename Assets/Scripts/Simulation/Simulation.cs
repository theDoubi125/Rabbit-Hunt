using System.Collections;
using System.Collections.Generic;
using UnityEngine;

struct SimulationOrder
{
    public CharacterSimulator Character;
    public List<Cell> Path;
}

class Simulation : ScriptableObject
{
    private List<CharacterSimulator> simulatedEntities = new List<CharacterSimulator>();
    public delegate void SimulationFinishedDelegate(Simulation simulation);

    public SimulationFinishedDelegate OnSimulationFinished;

    public void LaunchSimulation(List<SimulationOrder> orders)
    {
        foreach(SimulationOrder order in orders)
        {
            simulatedEntities.Add(order.Character);
            order.Character.OnSimulationFinished += OnCharacterSimulationFinished;
            order.Character.StartSimulation(order.Path);
        }
    }

    void OnCharacterSimulationFinished(CharacterSimulator entity)
    {
        entity.OnSimulationFinished -= OnCharacterSimulationFinished;
        simulatedEntities.Remove(entity);
        if(simulatedEntities.Count == 0)
        {
            Destroy(this);
            OnSimulationFinished(this);
        }
    }
}
