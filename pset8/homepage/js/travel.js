// Adapt display form to different picture and window size

document.addEventListener("DOMContentLoaded", function() {
    
    // When visiting the page
    function changeViewMode() {
        
        let menuHeight = document.querySelector(".header").offsetHeight;
        let pageContentHeight = document.documentElement.scrollHeight - menuHeight;
        let imageHeight = document.querySelector(".active img").offsetHeight;
        
        if (pageContentHeight > imageHeight) {
            document.querySelector(".carousel-item.active").classList.add("trigger");
        }
        
        else {
            document.querySelector(".carousel-item.active").classList.remove("trigger");
        }
    }
    
    window.onload = changeViewMode;
    
    
    // When changing to next image
    document.querySelectorAll(".carousel-item").forEach(img => {
        
        function workOnClassAdd() {
            
            let menuHeight = document.querySelector(".header").offsetHeight;
            let pageContentHeight = document.documentElement.scrollHeight - menuHeight;
            let imageHeight = document.querySelector(".active img").offsetHeight;
            
            if (pageContentHeight > imageHeight) {
                img.classList.add("trigger");
            }
        }
        
        function workOnClassRemoval() {
            img.classList.remove("trigger");
        }
        
        class ClassWatcher {
            
            // Only constructor function will run when "class" is created by "new" as an object
            constructor(targetNode, classToWatch, classAddedCallback, classRemovedCallback) {
                this.targetNode = targetNode;
                this.classToWatch = classToWatch;
                this.classAddedCallback = classAddedCallback;
                this.classRemovedCallback = classRemovedCallback;
                this.observer = null;
                this.lastClassState = targetNode.classList.contains(this.classToWatch);
                
                // Active next function when "class" object is created
                this.init();
            }
            
            init() {
                // "MutationObserver()" calls back "change report object"
                this.observer = new MutationObserver(this.mutationCallback);
                this.observe();
            }
            
            observe() {
                // ".observe" triggers a continuous detection for input target
                this.observer.observe(this.targetNode, { attributes: true });
            }
            
            /* Deactivate observer
            disconnect() {
                this.observer.disconnect();
            }*/
            
            mutationCallback = mutationsList => {
                for(let mutation of mutationsList) {
                    if (mutation.type === "attributes" && mutation.attributeName === "class") {
                        let currentClassState = mutation.target.classList.contains(this.classToWatch);
                        if(this.lastClassState !== currentClassState) {
                            this.lastClassState = currentClassState;
                            
                            if(currentClassState) {
                                this.classAddedCallback();
                            }
                            
                            else {
                                this.classRemovedCallback();
                            }
                        }
                    }
                }
            }
        }
      
        // Watch for a specific class="active" change
        let classWatcher = new ClassWatcher(img, "active", workOnClassAdd, workOnClassRemoval);
    });
    
    
    // When resizing window ("window.onresize" only triggers when resize starts)
    let resize;
    window.onresize = function() {
        clearTimeout(resize);
        resize = setTimeout(changeViewMode, 350);
    };
    
});