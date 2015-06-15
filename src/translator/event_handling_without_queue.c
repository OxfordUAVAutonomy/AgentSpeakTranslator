/* in the event-queue-less version, we have potentially two events:
 * 1) an event that needs to be completed, this one will always be given
 *    preference, if set
 * 2) a deferred event, this one will be used, if no event to complete
 *    is set
 */
static bool rtc_event_set;
static eventt rtc_event;
static bool deferred_event_set;
static eventt deferred_event;

/* initialise the events, there's not much to do here... */
static void init_events(void)
{
  rtc_event_set = false;
  deferred_event_set = false;
}

/* indicate whether another event can be generated */
bool can_add_event(void)
{
  return !rtc_event_set;
}

/* add deferred event */
static bool add_event_new_focus(eventt event)
{
  if (!deferred_event_set)
  {
    deferred_event = event;
    deferred_event_set = true;
    return true;
  }
  // TODO: This should not happen
  return false;
}

/* add event to complete */
static bool add_event_to_complete(eventt event)
{
  if (!rtc_event_set)
  {
    rtc_event = event;
    rtc_event_set = true;
    return true;
  }
  // TODO: This should not happen
  return false;  
}

/* without an event queue, we handle external events immediately, as they are supposed
   to be only added when can_add_event() returns true, i.e., no event_to_complete is set */
static bool add_external_event(eventt event)
{
  return add_event_to_complete(event);
}

/* get next event: event_to_complete is given preference over deferred_event */
static eventt get_next_event(void)
{
  eventt event = { NONE };
  if (rtc_event_set) 
  {
    event = rtc_event;
    rtc_event_set = false;
  }
  else if (deferred_event_set)
  {
    event = deferred_event;
    deferred_event_set = false;
  }
  return event;
}

