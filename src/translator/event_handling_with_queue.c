/* event-queue, a circular buffer, current_event points to the head */
static eventt events[MAX_EVENTS];
static unsigned int events_size;
static unsigned int current_event;

/* initialise the event queue */
static void init_events(void)
{
  events_size = 0;
  current_event = 0;
}

/* add event to event queue at given index */
static bool add_event(eventt event, unsigned int index)
{
  if (events_size < MAX_EVENTS) 
  {
    /* TODO: add overwrite check? */
    events[index] = event;
    events_size = events_size + 1;
    return true;
  }
  /* TODO: what to do in this case? */
  return false;
}

/* add event at tail */
static bool add_event_new_focus(eventt event)
{
  unsigned int tail = (current_event + events_size) % MAX_EVENTS;
  return add_event(event, tail);
}

/* add event at head */
static bool add_event_to_complete(eventt event)
{
  /* avoid problems with integer division and % */
  unsigned int head;
  if (current_event == 0)
  {
    head = MAX_EVENTS-1;
  }
  else
  {
    head = current_event - 1;
  }
  if (!add_event(event, head)) 
  {
    return false;
  }
  current_event = head;
  return true;
}

/* with an event queue, external events are queued at the back of the event queue */
bool add_external_event(eventt event)
{
  return add_event_new_focus(event);
}

/* pop next event from event queue (at head) */
static eventt get_next_event(void)
{
  if (events_size <= 0) 
  {
    eventt nothing = { NONE };
    return nothing;
  }
  eventt event = events[current_event];
  current_event = (current_event + 1) % MAX_EVENTS;
  events_size = events_size - 1;
  return event;
}

/* indicate whether event queue is full or not */
bool can_add_event(void)
{
  return events_size < MAX_EVENTS;
}

