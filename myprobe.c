/*
 * @filename:    myprobe.c
 * @author:      Stefan Stockinger
 * @date:        2022-06-25
 * @description:
*/

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/kprobes.h>
#include <linux/init.h>


static char *hook;

module_param(hook, charp, 0);
MODULE_PARM_DESC(hook,"please pass the function name, where you want to install your probe");


static struct kprobe kp;
static int handle_pre(struct kprobe *kp, struct pt_regs *regs){

 // ref: https://elixir.bootlin.com/linux/v5.13/source/include/linux/sched.h#L657
 printk(KERN_INFO "kprobe: Prozess %s hat %s gerufen mit UID %d\n", current->comm, kp->symbol_name, current->cred->uid.val);
 printk(KERN_INFO "kprobe: ip=%ld, pid=%d, tgid=%d\n", regs->ip, current->pid, current->tgid);
 printk(KERN_INFO "kprobe: state=%ld [ -1 unrunnable, 0 runnable, >0 stopped]\n",current->state);
 printk(KERN_INFO "kprobe: counter=%ld, rt_priority= %d, prio=%d,  normal_prio=%d, static_prio=%d\n",current->state, current->rt_priority, current->prio, current->static_prio, current->normal_prio);
 printk(KERN_INFO "kprobe: flags=%d",current->flags);
 printk(KERN_INFO "kprobe: nr_cpus_allowed=%d, cpu=%d, recent_used_cu=%d, wake_cpu=%d\n", current->nr_cpus_allowed, current->cpu, current->recent_used_cpu, current->wake_cpu);
 printk(KERN_INFO "kprobe: exit_state=%d, exit_code=%d,  exit_signal=%d, pdeath_signal=%d\n",current->exit_state,current->exit_code,current->exit_signal,current->pdeath_signal);
 printk(KERN_INFO "kprobe: contect switches nvcsw=%ld, nivcsw=%ld\n",current->nvcsw, current->nivcsw);
 printk(KERN_INFO "kprobe: utime=%lld, stime=%lld, start_time=%lld, start_boottime=%lld\n",current->utime, current->stime, current->start_time, current->start_boottime);
 printk(KERN_INFO "kprobe: page_faults: min_flt=%ld, maj_flt=%ld\n",current->min_flt, current->maj_flt);
 printk(KERN_INFO "kprobe: credentials address: ptracer_cred=%p, real_cred=%p, cred=%p\n",current->ptracer_cred,current->real_cred,current->cred);
 printk(KERN_INFO "kprobe: fs=%p, files=%p, io_context=%p",current->fs, current->files, current->io_context);
 printk(KERN_INFO "kprobe: stack dump:\n");
 dump_stack();
 return 0;
}

static void handle_post(struct kprobe *kp, struct pt_regs *regs, unsigned long l){
 return;
}


static int handle_fault(struct kprobe *kp, struct pt_regs *regs, int trap){
 printk(KERN_ERR "kprobe: ProbeFault %d\n", trap);
 return 0;
}


static int __init kprobe_init(void){
  int ret;
  kp.pre_handler = handle_pre;
  kp.post_handler = handle_post;
  kp.fault_handler = handle_fault;
  kp.symbol_name = hook;
  
  ret = register_kprobe(&kp);
  if(ret<0){
      printk(KERN_INFO "kprobe: Failed load kprobe\n");
      return ret;
  }
  printk(KERN_INFO "kprobe: installed - hook on '%s'\n", kp.symbol_name);
  return 0;    
}

static void __exit kprobe_exit(void){
  unregister_kprobe(&kp);
  printk(KERN_INFO "kprobe: exit\n");
}

module_init(kprobe_init)
module_exit(kprobe_exit)
MODULE_LICENSE("GPL");
