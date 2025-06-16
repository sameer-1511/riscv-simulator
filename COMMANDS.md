# Commands

- `modify_config` or `mconfig`: `Section`, `Key`, `Value`
  - Modifies the internal configuration by setting the specified key in the given section to the provided value.
  - `Execution`
    - `processor_type` (string) : `single_stage` | `multi_stage`  
    - `run_step_delay` (unsigned int) : milliseconds  
  - `Memory`
    - `memory_size` (unsigned int) : bytes
    - `memory_block_size` (unsigned int) : bytes  
