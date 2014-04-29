Finite State Machine
--------------------
A machine is a device which accepts some inputs, possibly produces some output,
and has some memory sorting information on the overall results of all previous
inputs.

The condition of the machine at a particular instance is called a state of the
machine. New input changes the state, may be to another state, and may be to
the same state. The effect of a new input depends on the present state the
machine is in.

If the total number of possible states is finite, the machine is called finite
state machine, eg. a computer.

More information about State Machines can be found here: http://en.wikipedia.org/wiki/Finite-state_machine

Event Execution Summary
-----------------------
1. Check the event's guard condition.  Stop here if FALSE.
2. Event::before_transition
3. CurrentState::on_exit
4. Update state in machine
5. NewState::on_enter
6. Event::after_transition


Structure of a State Machine
----------------------------
A StateMachine is made up of States and Events.
States are the status of the item at a certain point in time.
Events describe how the machine can move between states.


State Flow Node
-----------------------
State Flow Node is the base implementation of State Machine class. It provides a base workflow as a base plugin for nodes.

Custom plugins should use State Flow Node as a model when:

Adding new states
Adding new events


Integration
-----------------------
State Flow Node provides the following optional integration with other modules:

Rules: Hook a condition into a event transition
Views: Exposes defined states, revision information, and timestamps


Workbench Moderation 2.x Integration
-----------------------
State Flow Entity was instituted to allow workflows for any type of entity. State Flow Node uses State Flow Entity for nodes. State Flow Entity was designed to
provide a base for Workbench Moderation 2.x
